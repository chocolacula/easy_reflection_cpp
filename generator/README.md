# er_gen

Code generation tool for Easy Reflection based on Clang tools. It combines parsed data into one json entity and forwards it to inja tempate engine.

The format of object entity is

```json
{
  "kind" : 0, // 0 for objects
  "name" : string,  // full name including all namespaces
  "file_name" : string, // full name converted to snake_case with extension
  "origin" : string, // file path to origin file for analysis
  "bases" : [
    {
      "access" : [ "kPublic" | "kProtected" | "kPrivate"],
      "name" : string, // full name including all namespaces
    }
  ],
  "fields" : [
    {
      "acc" : [ "kPublic" | "kProtected" | "kPrivate" | "kConst" | "kStatic" ],
      "name" : string,
      "alias" : string
    }
  ],
  "methods" : [
    {
      "acc" : [ "kPublic" | "kProtected" | "kPrivate" | "kConst" | "kStatic" ],
      "name" : string,
      "alias" : string
    }  
  ],
}
```

format of an enum entity is

```json
{
  "kind" : 1, // 1 for enums
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
