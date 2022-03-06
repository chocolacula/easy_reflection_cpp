# rr_gen

Code generation tool for Easy Reflection based on Clang tools.

It combines parsed data into one json entity and forwards it to inja tempate engine.

format of an object entity is

```json
{
  "id" : 0, // 0 is object id
  "name" : string,  // full name including all namespaces
  "file_name" : string, // full name converted to snake_case with extension
  "origin" : string, // file path to origin file for analysis
  "bases" : [
    {
      "access" : int,
      "name" : string,
    }
  ],
  "fields_static" : [
    {
      "access" : int,
      "name" : string,
      "alias" : string // equal "name" if ER_ALIAS attribute didn't parse
    }
  ],
  "fields" : [
    {
      "access" : int,
      "name" : string,
      "alias" : string
    }
  ]
}
```

format of an enum entity is

```json
{
  "id" : 1, // 1 is enum id
  "name" : string,  // full name including all namespaces
  "file_name" : string, // full name converted to snake_case with extension
  "origin" : string, // file path to origin file for analysis
  "constants" : [
    {
      "name" : string,
      "alias" : string
    }
  ]
}
```
