# neo_font_editor

Edit alphasmart neo and neo 2 fonts

Edit font by dumping to text file, edit and encode back again

used together with  [neotools](https://github.com/lykahb/neotools) to extract
and install fonts.

## Workflow

1. Build the project with cmake
2. Use neotools to extract your font applet
3. run `font_editor your_font.os3kapp -f font-data.txt
4. create a remap-file with the characters you want to move
5. change appletname and id and other information in your font-data file
6. encode your app back to a applet `font_editor --encode -f font-data.txt -o new-font-location.os3kapp --remap remap-scheme-uk-sv-dvorak.txt`
7. install the new code on your neo with neotools

## Example remap file

To find what character is what look inside the dumped text file

```python

# define chearacters
(  40
)  41
,  44

# Define which keys should replace which
# First the key you want -> then what character it should replace
, -> )
) -> (
) -> ,
```

## Example editable file:

```

appletname Neo Font - Small (6 lines)
appletinfo _opyright (c) 19?8-2005 by Alpha_mart, LLC
fontname Small (6 lines)
version 1.0
id 44800
height 11
char 0
width 5
height 11

xxxxx.
xxxxx.
xxxxx.
xxxxx.
xxxxx.
xxxxx.
xxxxx.
xxxxx.
xxxxx.
xxxxx.
xxxxx.

char 1
width 5
height 11

     .
 xx  .
x  x .
 x   .
x x  .
x  x .
x  x .
 xx  .
     .
     .
     .

char 2
width 8
height 11

        .
   x    .
  x x   .
  x x   .
 x   x  .
 x   x  .
x     x .
xxxxxxx .
        .
        .
        .
       
... etc
```
