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

```Arrays```, ```Sequences```, ```Maps``` and ```Strings``` writes length number before their content while serialization.

<b>IMPORTANT:</b> strings are serializing a little bit different. Standard ```std::string``` stores
a character in one byte and <b>VLQ</b> technique doesn't make sense. For this reason strings write it's own content one sequential bunch of bytes. If string length is the first subheader in the header, the second one will leave empty. Otherwise we will have to go back and brake CPU cache to write the second one subheader and then go forward and brake the cache again.