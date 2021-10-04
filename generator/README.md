# rr_gen

Code generation tool for Easy Reflection based on Clang tools.

It combines parser info into one json entity and forward it to inja tempate engine.

format of an object entity is

```js
{
  "id" : 0, // 0 is object id
  "name" : string,  // full name including all namespaces
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

```js
{
  "id" : 1, // 1 is enum id
  "name" : string,  // full name including all namespaces
  "origin" : string, // file path to origin file for analysis
  "constants" : [
    {
      "name" : string,
      "alias" : string
    }
  ]
}
```