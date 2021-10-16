### Binary serialization

It employs <b>Variable Length Quantity</b> to optimize number of serialized bytes.

For more information see [this wiki article](https://en.wikipedia.org/wiki/Variable-length_quantity)

The implemented approach stores data by groups, one group is:

| header | first value | second value |
| ---    | ---         | ---          |
| 8 bits | 1 - 8 bytes | 1 - 8 bytes  |

Where is header has two words as sub headers for each value:

| sign   bit | bytes number | sign   bit | bytes number |
| ---        | ---          | ---        | ---          |
| 1 bit      | 3 bits<br> 1(```0b000```) - 8(```0b111```) | 1 bit | 3 bits <br> 1(```0b000```) - 8(```0b111```) |

In one cache line(64 bytes) placed 3 groups maximum, min 3 bytes max 17 bytes.