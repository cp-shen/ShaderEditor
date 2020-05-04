/****************************************/
/* Date: 2020-05-03                     */
/* Desc: utility code about text editor */
/****************************************/
#ifndef EDITOR_H
#define EDITOR_H

#include <TextEditor.h>

void editor_setup(TextEditor &editor);
void editor_save_text(TextEditor &editor, const char *file_path);
void editor_load_file(TextEditor &editor, const char *file_path);

#endif
