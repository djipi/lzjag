This archive contains four folders containing data necessary for doing LZSS
compression on the Jaguar as follows:

NEWINCS
-------
WARNING!!! This set of files was compiled using versions of include files
you probably aren't using and shouldn't use to overwrite your existing
installation. They are provided so that if the files won't remake without
error you can compare with them to see what the dilemma is.

PC.SRC
------
This is the PC .EXE LZSS Compressor/Decompressor. A command like:

lzjag -c file.bin

will create a file named FILE.LZJ that is compressed. A command like:

lzjag -x file.lzj

will create a file named FILE.BIN that is decompressed. Please note that
LZJ files have no header and thus you could potentially try to decompress
a file that isn't actually compressed and get strange results.

The source code is included if you need it for some reason. It is still
in the development stages so please keep that in mind. It was built using
Microsoft C/C++ 8.0 in the Huge memory model.

JAG.SRC
-------
This directory contains the file DELZJAG.S which should be linked with your
code. To use the Jaguar GPU compressor you must:

1. Copy (or better yet Blit) the GPU code from the ROM into GPU RAM. It is
   currently assembled to run at $F03100 but that can be changed by
   changing the .org statement appropriately.

2. Write the address of your compressed data and an output buffer to
   addresses lzinbuf and lzoutbuf respectively.

3. Write the address of a usable 8k block of buffer memory to lzworkbuf.

4. Load the GPU PC counter with the address of the routine (delzss) and
   start the GPU

5. If you have other things to do that take time and don't use the GPU
   or any of the above buffers, do them.

6. Finally, poll the G_CTRL register to ensure the decompression is finished.

Things to be aware of: This subroutine does not use a stack but does use
(and trash) registers r0-r13 (most certainly more in the next revision)
though you can start the GPU in either register bank.


TEST
----
The files in this directory make to form a short stub that follows the
above procedure to decompress a 4k block of compressed data into 26k of
code at $100000 and then executes it (it displays the Jaguar logo).

If you have any questions, please contact me at (408) 745-2143.
Compuserve: 70007,1135

-Scott @ Atari Corp.
