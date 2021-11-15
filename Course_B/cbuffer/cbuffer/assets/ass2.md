# C Assignment 2

## Requirements

Implement a circular buffer according to the requirements below. Download the zip file attached here and implement the functions.

1. The buffer is an array of type uint8_t
2. The module shall have a clear function to clear the buffer
3. It shall be possible to have a function to read data from the buffer
4. It shall be possible to have a function to peek data in the buffer
5. It shall be possible to have a function to write data to the buffer
6. It shall be possible to have a function to return the number of avaiable data in the buffer to read
7. It shall be possible to have a function to check if the buffer is full or not
8. Don’t change the header file or include extra libraries in the implementation file of the module.

## Fullfillment of requirements

0. Requirements do not specify what kind of data structure the circular buffer shall be implemented as. Stack or queue, who knows? I made a FIFO (or queue) because that's a common use for circular buffers.

1. Fulfilled

2. Fulfilled

3-7. The phrasing "It shall be possible to have a" does not appear in RFC2119 nor any other standards document I have been able to find. How does this phrasing relate to phrasing in req 2? Should the functions be implemented or should I just leave the header declarations? Even without any code it's very possible to have those functions. I decided to just implement them anyway because whatever.

8. Fulfilled, although the requirement precludes any kind of reasonable error checking.
    * Can't use any error checking library.
    * Can't allow caller to provide pointer to error code variable.
    * Can't even return a negative value for caller to check for.

    Because of this, when reading or peeking empty buffer it simply returns 0. Therefore the buffer cannot safely be used to store the value 0, because then there is no way for caller to know if value 0 really was in buffer or the result of an empty buffer.



Reqs 3-7: "It shall be possible to have a function to"

Test req 5: "when the buffer is not empty and full"

