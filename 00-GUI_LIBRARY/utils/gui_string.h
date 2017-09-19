/**
 * \author  Tilen Majerle <tilen@majerle.eu>
 * \brief   GUI string functions
 *  
\verbatim
   ----------------------------------------------------------------------
    Copyright (c) 2017 Tilen Majerle

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software, 
    and to permit persons to whom the Software is furnished to do so, 
    subject to the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
    AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
   ----------------------------------------------------------------------
\endverbatim
 *
 */
#ifndef GUI_STRING_H
#define GUI_STRING_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup      GUI_UTILS
 * \brief       
 * \{
 */
#include "gui_utils.h"

/**
 * \defgroup        GUI_STRING String functions
 * \brief           String functions with UNICODE support
 * \{
 */
    
/**
 * \defgroup        GUI_STRING_UNICODE Unicode processing
 * \brief           Unicode processing functions with UTF-8 character encoding
 *
 * Core functions capable of encoding and decoding UTF-8 UNICODE format.
 *
 * Decoding of raw unicode bytes can take up to 4 bytes in a row and stores value to 32-bit variable
 *
 * \par UNICODE structure table
 *
 * <table>
 *  <tr><th>Number of bytes <th>Bits for code point <th>First code points   <th>Last code point <th>Byte 1      <th>Byte 2      <th>Byte 3      <th>Byte 4
 *  <tr><td>1               <td>7                   <td>U+0000              <td>U+007F          <td>0xxxxxxx    <td>-           <td>-           <td>-
 *  <tr><td>2               <td>11                  <td>U+0080              <td>U+07FF          <td>110xxxxx    <td>10xxxxxx    <td>-           <td>-
 *  <tr><td>3               <td>16                  <td>U+0800              <td>U+FFFF          <td>1110xxxx    <td>10xxxxxx    <td>10xxxxxx    <td>-
 *  <tr><td>4               <td>21                  <td>U+10000             <td>U+10FFFF        <td>11110xxx    <td>10xxxxxx    <td>10xxxxxx    <td>10xxxxxx
 * </table>
 *
 * https://en.wikipedia.org/wiki/UTF-8
 *
 * \{
 */
    
/**
 * \brief           UNICODE processing information structure
 */
typedef struct GUI_STRING_UNICODE_t {
    uint8_t t;                                      /*!< Total number of bytes in UTF-8 sequence */
    uint8_t r;                                      /*!< Remaining bytes in UTF-8 sequnce before we get valid data */
    uint32_t res;                                   /*!< Current result in UTF8 sequence */
} GUI_STRING_UNICODE_t;

/**
 * \brief           UNICODE processing result information
 */
typedef enum GUI_STRING_UNICODE_Result_t {
    UNICODE_OK = 0x00,                              /*!< Indicates function successfully decode unicode sequence of 1-4 bytes */
    UNICODE_ERROR,                                  /*!< Indicates function got an error with input data from 2nd to 4th byte in UTF-8 encoding */
    UNICODE_PROGRESS,                               /*!< Indicates function decoding is still in progress and it waits for new character */
} GUI_STRING_UNICODE_Result_t;

/**
 * \brief           Initialize unicode processing structure
 * \param[in]       *s: Pointer to \ref GUI_STRING_UNICODE_t to initialize to default values
 * \retval          None
 * \sa              gui_string_unicode_decode, gui_string_unicode_encode
 */
void gui_string_unicode_init(GUI_STRING_UNICODE_t* s);

/**
 * \brief           Decodes single input byte of unicode formatted text
 * \param[in,out]   *s: Pointer to working \ref GUI_STRING_UNICODE_t structure for processing
 * \param[in]       c: Character to be used for decoding
 * \retval          Member of \ref GUI_STRING_UNICODE_Result_t indicating decoding status
 * \sa              gui_string_unicode_init, gui_string_unicode_encode
 */
GUI_STRING_UNICODE_Result_t gui_string_unicode_decode(GUI_STRING_UNICODE_t* s, const GUI_Char c);

/**
 * \brief           Encodes input character to UNICODE sequence of 1-4 bytes
 * \param[in]       c: Character to encode to UNICODE sequence
 * \param[out]      *out: Pointer to 4-bytes long array to store UNICODE information to
 * \retval          Number of bytes required for character encoding
 * \sa              gui_string_unicode_init, gui_string_unicode_decode
 */
uint8_t gui_string_unicode_encode(const uint32_t c, GUI_Char* out);

/**
 * \}
 */
 
/**
 * \brief           String structure for parsing characters
 */
typedef struct GUI_STRING_t {
    const GUI_Char* Str;                /*!< Pointer to source string */
#if GUI_USE_UNICODE || defined(DOXYGEN)
    GUI_STRING_UNICODE_t S;             /*!< Unicode processing structure */
#endif /* GUI_USE_UNICODE || defined(DOXYGEN) */
} GUI_STRING_t;

/**
 * \brief           Return length of string
 *      
 *                  Example input string: EasyGUI\\xDF\\x8F
 *
 *                  1. When \ref GUI_USE_UNICODE is set to 1, function will try to parse unicode characters
 *                      and will return 8 on top input string
 *                  2. When \ref GUI_USE_UNICODE is set to 0, function will count all the bytes until string end is reached
 *                      and will return 9 on top input string
 *
 * \param[in]       *src: Pointer to source string to get length
 * \retval          Number of visible characters in string
 * \sa              gui_string_lengthtotal
 */
size_t gui_string_length(const GUI_Char* src);

/**
 * \brief           Return total number of bytes required for string
 *      
 * \note            When \ref GUI_USE_UNICODE is set to 0, this function returns the same as \ref gui_string_length
 *
 * \param[in]       *src: Pointer to source string to get length
 * \retval          Number of visible characters in string
 * \sa              gui_string_length
 */
size_t gui_string_lengthtotal(const GUI_Char* src);

/**
 * \brief           Copy string from source to destination no matter of \ref GUI_USE_UNICODE selection
 * \param[out]      *dst: Destination memory address
 * \param[in]       *src: Source memory address
 * \retval          Pointer to destination memory
 * \sa              gui_string_copyn
 */
GUI_Char* gui_string_copy(GUI_Char* dst, const GUI_Char* src);

/**
 * \brief           Copy string from source to destination with selectable number of bytes
 * \param[out]      *dst: Destination memory address
 * \param[in]       *src: Source memory address
 * \param[in]       len: Number of bytes to copy
 * \retval          Pointer to destination memory
 * \sa              gui_string_copy
 */
GUI_Char* gui_string_copyn(GUI_Char* dst, const GUI_Char* src, size_t len);

/**
 * \brief           Compare 2 strings
 * \param[in]       *s1: First string address
 * \param[in]       *s2: Second string address
 * \retval          0: Strings are the same
 * \retval          !=0: Strings are not the same
 */
int gui_string_compare(const GUI_Char* s1, const GUI_Char* s2);

/**
 * \brief           Check if character is printable
 * \param[in]       ch: First memory address
 * \retval          1: Character is printable
 * \retval          0: Character is not printable
 */
uint8_t gui_string_isprintable(uint32_t ch);

/**
 * \brief           Prepare string before it can be used with \ref GUI_STRING_GetCh or \ref GUI_STRING_GetChReverse functions
 * \param[in,out]   *s: Pointer to \ref GUI_STRING_t as base string object
 * \param[in]       *str: Pointer to \ref GUI_Char with string used for manupulation
 * \retval          1: String prepared and ready to use
 * \retval          0: String was tno prepared
 */
uint8_t gui_string_prepare(GUI_STRING_t* s, const GUI_Char* str);

/**
 * \brief           Get next decoded character from source string
 *
 * \code{c} 
GUI_Char myStr[] = "EasyGUI\xDF\x8F\xDF\x8F";   //Source string to check
GUI_STRING_t s;                                 //Create string variable
uint32_t ch;                                    //Output character
uint8_t i;                                      //Number of bytes required for character generation

//GUI_USE_UNICODE = 1: string length = 9;  Length total: 11
//GUI_USE_UNICODE = 0: string length = 11; Length total: 11
gui_string_prepare(&s, myStr);                  //Prepare string for reading
while (gui_string_getch(&s, &ch, &i)) {         //Go through entire string
    printf("I: %d, ch %c (%d)\r\n", i, ch, ch); //Print character by character
}
\endcode
 * 
 * \note            When \ref GUI_USE_UNICODE is set to 1, multiple bytes may be used for single character
 * \param[in,out]   *str: Pointer to \ref GUI_STRING_t structure with input string. 
                        Function will internally change pointer of actual string where it points to to next character
 * \param[out]      *out: Pointer to output memory where output character will be saved
 * \param[out]      *len: Pointer to output memory where number of bytes for string will be saved
 * \retval          1: Character decoded OK
 * \retval          0: Error with character decode process or string has reach the end
 * \sa              gui_string_getchreverse
 */
uint8_t gui_string_getch(GUI_STRING_t* str, uint32_t* out, uint8_t* len);

/**
 * \brief           Get character by character from end of string up
 * \note            Functionality is the same as \ref GUI_STRING_GetCh except order is swapped
 * 
 * \note            String must be at the last character before function is first time called
 *
 * \code{c}
 //TODO: Update code!
GUI_Char myStr[] = "EasyGUI\xDF\x8F\xDF\x8F";   //Source string to check
GUI_STRING_t s;                                 //Create string variable
uint32_t ch;                                    //Output character
uint8_t i;                                      //Number of bytes required for character generation

//GUI_USE_UNICODE = 1: string length = 9;  Length total: 11
//GUI_USE_UNICODE = 0: string length = 11; Length total: 11
gui_string_prepare(&s, myStr);                  //Prepare string for reading
gui_string_gotoend(&ptr);                       //Go to last character of string
while (gui_string_getchreverse(&s, &ch, &i)) {  //Go through entire string
    printf("I: %d, ch %c (%d)\r\n", i, ch, ch); //Print character by character
}
\endcode
 *
 * \note            When \ref GUI_USE_UNICODE is set to 1, multiple bytes may be used for single character
 * \param[in,out]   *str: Pointer to \ref GUI_STRING_t structure with input string. 
                        Function will internally change pointer of actual string where it points to to next character
 * \param[out]      *out: Pointer to output memory where output character will be saved
 * \param[out]      *len: Pointer to output memory where number of bytes for string will be saved
 * \retval          1: Character decoded OK
 * \retval          0: Error with character decode process or string has reach the start
 * \sa              gui_string_getch, gui_string_gotoend
 */
uint8_t gui_string_getchreverse(GUI_STRING_t* str, uint32_t* out, uint8_t* len);

/**
 *
 * \brief           Set character pointer to the last character in sequence
 * \param[in,out]   **str: Pointer to pointer to source string to modify pointing location
 * \retval          1: Pointer set to last character
 * \retval          0: Pointer was not set to last character
 * \sa              gui_string_getchreverse
 */
uint8_t gui_string_gotoend(GUI_STRING_t* str);
    
/**
 * \}
 */

/**
 * \}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
