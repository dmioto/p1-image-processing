# PGM Image processing

Program Functions:

- (1) Thresholding
- (2) Negative
- (3) Histogram
- (4) Run all above

## Setup to run

1. Clone this repository.

   ```shell
   $ git clone https://github.com/dmioto/p1-image-processing.git
   $ cd p1-image-processing
   ```

2. Now you can build the executable with gcc or just run the executable passing the .pgm image file as argument.

   To build:
  
   ```shell
   $ gcc -o program convert_image.c
   $ ./program <pgm-image-path.pgm>
   ```
 
   Or just use the executable a.out in repository:
 
   ```shell
   $ ./a.out <pgm-image-path.pgm>
   ```
