/****************************************/
/* Date: 2020-05-03                     */
/* Desc: utility code about text editor */
/****************************************/
#ifndef EDITOR_H
#define EDITOR_H

#include <TextEditor.h>

void editor_setup(const char *file_path);
void editor_save_text( const char *file_path);
void process_editor_main_loop();

#endif
