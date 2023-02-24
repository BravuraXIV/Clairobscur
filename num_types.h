/*---------------------------------------------------------------------
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/> 
---------------------------------------------------------------------*/

#ifndef NUM_TYPES_H
#define NUM_TYPES_H

#ifdef __cplusplus
#define NUM_TYPES_CUSTOM_NAMESPACE
namespace NUM_TYPES_CUSTOM_NAMESPACE
{
#endif // __cplusplus

using s8 = char;
using u8 = unsigned char;

using s16 = short;
using u16 = unsigned short;

////////////// Clang //////////////
#ifdef __clang__

using s32 = __INT32_TYPE__;
using u32 = unsigned __INT32_TYPE__;

using s64 = __INT64_TYPE__;
using u64 = unsigned __INT64_TYPE__;

#endif // __clang__
///////////////////////////////////


/////// Microsoft Visual C ////////
#ifdef _MSC_VER

using s32 = __int32;
using u32 = unsigned __int32;

using s64 = __int64;
using u64 = unsigned __int64;

#endif // _MSC_VER
///////////////////////////////////

// TODO: GCC

using f32 = float;
using f64 = double;

#ifdef __cplusplus
} // namespace NUM_TYPES_CUSTOM_NAMESPACE
#endif // __cplusplus
    
#endif
