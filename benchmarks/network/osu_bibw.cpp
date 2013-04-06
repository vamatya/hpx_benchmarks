//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Bidirectional network bandwidth test

#include <hpx/hpx_main.hpp>
#include <hpx/hpx.hpp>
#include <hpx/include/iostreams.hpp>
#include <hpx/util/serialize_buffer.hpp>

#include <boost/assert.hpp>
#include <boost/shared_ptr.hpp>

///////////////////////////////////////////////////////////////////////////////
#define LOOP_SMALL  100
#define WINDOW_SIZE_SMALL  64
#define SKIP_SMALL  10

#define LOOP_LARGE  20
#define WINDOW_SIZE_LARGE  64
#define SKIP_LARGE  2

#define LARGE_MESSAGE_SIZE  8192

#define MAX_MSG_SIZE (1<<22)
#define MAX_ALIGNMENT 65536
#define SEND_BUFSIZE (MAX_MSG_SIZE + MAX_ALIGNMENT)

char send_buffer[SEND_BUFSIZE];

///////////////////////////////////////////////////////////////////////////////
char* align_buffer (char* ptr, unsigned long align_size)
{
    return (char*)(((std::size_t)ptr + (align_size - 1)) / align_size * align_size);
}

#if defined(BOOST_MSVC)
unsigned long getpagesize()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return si.dwPageSize;
}
#endif

///////////////////////////////////////////////////////////////////////////////
void isend(hpx::util::serialize_buffer<char> const& receive_buffer) {}
HPX_PLAIN_ACTION(isend);

///////////////////////////////////////////////////////////////////////////////
double ireceive(hpx::naming::id_type dest, std::size_t size)
{
    int loop = LOOP_SMALL;
    int skip = SKIP_SMALL;
    int window_size = WINDOW_SIZE_SMALL;

    if (size > LARGE_MESSAGE_SIZE) {
        loop = LOOP_LARGE;
        skip = SKIP_LARGE;
        window_size = WINDOW_SIZE_LARGE;
    }

    // align used buffers on page boundaries
    unsigned long align_size = getpagesize();
    BOOST_ASSERT(align_size <= MAX_ALIGNMENT);

    char* aligned_send_buffer = align_buffer(send_buffer, align_size);
    std::memset(aligned_send_buffer, 'a', size);

    hpx::util::high_resolution_timer t;
    for (int i = 0; i != loop + skip; ++i) {
        // do not measure warm up phase
        if (i == skip)
            t.restart();

        std::vector<hpx::future<void> > lazy_results;
        lazy_results.reserve(window_size);

        isend_action send;
        for (int j = 0; j < window_size; ++j)
        {
            typedef hpx::util::serialize_buffer<char> buffer_type;

            // Note: The original benchmark uses MPI_Isend which does not
            //       create a copy of the passed buffer.
            lazy_results.push_back(hpx::async(send, dest,
                buffer_type(aligned_send_buffer, size, buffer_type::reference)));
        }
        hpx::wait_all(lazy_results);
    }

    double elapsed = t.elapsed();
    return (size / 1e6 * loop * window_size) / elapsed;
}
HPX_PLAIN_ACTION(ireceive);

///////////////////////////////////////////////////////////////////////////////
void print_header ()
{
    hpx::cout << "# OSU HPX Bi-Directional Test\n"
              << "# Size    Bandwidth (MB/s)\n"
              << hpx::flush;
}

///////////////////////////////////////////////////////////////////////////////
void run_benchmark()
{
    // use the first remote locality to bounce messages, if possible
    hpx::id_type here = hpx::find_here();

    hpx::id_type there = here;
    std::vector<hpx::id_type> localities = hpx::find_remote_localities();
    if (!localities.empty())
        there = localities[0];

    // perform actual measurements
    ireceive_action receive;
    for (std::size_t size = 1; size <= MAX_MSG_SIZE; size *= 2)
    {
        hpx::future<double> receive_there = hpx::async(receive, there, here, size);
        hpx::future<double> receive_here = hpx::async(receive, here, there, size);

        std::vector<hpx::future<double> > band_widths =
            hpx::wait_all(receive_there, receive_here);

        double bw = band_widths[0].get() + band_widths[1].get();
        hpx::cout << std::left << std::setw(10) << size
                  << bw << hpx::endl << hpx::flush;
    }
}

int main(int argc, char* argv[])
{
    print_header();
    run_benchmark();
    return 0;
}
