# UbexCpp
## A High Performance, compact, and intuitive JSON-style binary serialization library

#### This is clean C++14 implementation of the Uncomplicated Binary Exchange Format (UBEXF) and Value semmantics

#### How?
The UBEXF is a simpler variant of UBJSON (A binary version of JSON). UBEXF carries simplicity of representing any data in JSON style and the efficiency of binary compactness. However it does not strive a 1:1 compatibility with JSON like UBJSON does. Please See Format Spec for details

#### About This Library
UbexCpp is a UBEXF Library that enables a client to seamlessly stream In/Out UBEXF encoded data from a StreamSource. The StreamSource is a template type that can be substituted provided it meets the Concept requirements. For example, std::ostream, std::istream and Boost.Asio can seamless be plugged in.

This library has been implemented using *C++14*, as part of the timl (Timothy's Libraries) for public use.

The Value type is an intuitive union type that is used to hold any UBEXF type, which includes int, char, strings, floats, binary, etc. See documentation for details

#### Usage Snippet
```C++
using namespace timl;
Value v1, v2;
v1 = 34535;
v1 = "I so much love C++14";
v2 = v1;

std::string s1 = std::move(v1);

auto b = s1 == static_cast<std::string>(v2);
v2 = -2342.2525236;

std::string s2 = v2.asString;   //Value::asString() exception safe unconditional cast
std::cout << v2.asInt() << std::endl;   //truncate to Integer....

```
If you are familiar with JsonCpp, using this library shouldn't be a problem :-).



Key-Value pairs or Maps? ...perfect
```C++
using namespace timl;
Value m1("country", "Nigeria");
Value m2;
m2["country"] = "Nigeria";
m2["location"]["latitude"]["relative"] = 34.2523; //creates the maps on the fly... fast

m1 == m2;   //Compare an Value type;
```



Arrays? ...No problems
```C++
using namespace timl;
Value array = {"Timothy", 2015, -34253535.235235, '@', Value("country", "Nigeria")};
array.push_back("Amazing Array");

for(auto val : array)
  std::cout << val.asString() << std::endl;

if(array.contains(2015))
  array.remove(2015)
```



Value also has binary types:
```C++
using namespace timl;
using Binary = Value::BinaryType;   //Actually, an alias for std::vector<unsigned char>
Value binary = Binary({0xF3, 0x33, 0x76, 0xAA, 0x23});
```



#### Stream Operations
Reading from a Stream is very simple.
```C++
  std::ifstream input;
  input.open("sample1.ubex", ios::binary);

  StreamReader<decltype(input)> reader(input);
  Value val = reader.getNextValue();

  //or
  StreamReader<std::ifstream> reader(input); ///your choice :-)
```



Writing to a Stream is likewise very simple.
```C++
  Value planet;
  planet["name"] = "Earth";
  planet["position"] = 3;

  std::ofstream output;
  output.open("sample2.ubex", ios::binary);

  StreamWriter<std::ostream> writer(output);
  auto result = writer.writeValue(planet);

  if(result.second)
    std::cout << "Successfully wrote: " << result.first << " bytes" << std::endl;
```


Pretty Printing.... easy:
```C++
Value value;
value["name"] = "Timothy"
value["country"] = "Nigeria";
value["favorites'] = { 34.351, -253, '@', value["country"], 34, value};

std::cout << to_ostream(value) << std::endl;
```




Written and authored by Ibrahim Timothy Onogu.
Please drop a comment.
