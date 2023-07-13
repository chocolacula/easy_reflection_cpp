#pragma once

/// options are:
/// WithBase - to reflect base class accessible fields too
/// WithNonPublic - to add private and protected fields to the reflected fields
/// DataOnly - reflect fields, skip methods
/// MethodsOnly - reflect methods, skip fields
#define ER_REFLECT(...)

#define ER_ALIAS(name)

#define ER_EXCLUDE
