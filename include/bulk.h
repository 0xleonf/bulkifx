#ifndef BULK_H
#define BULK_H

#include "core.h"

typedef struct {
  char **files; // array of filenames
  int count;    // total file
} FileList;

FileList *get_images_from_dir(const char *dispatch);
void free_file_list(FileList *list);
void process_bulk(const char *input_dir, const char *output_dir,
                  void (*process_func)(Image *));
void process_bulk_with_params(const char *input_dir, const char *output_dir,
                              void (*process_func)(Image *, void *),
                              void *params);

void bulk_brightness(Image *img, void *params);
void bulk_grayscale(Image *img, void *params);
void bulk_invert(Image *img, void *params);
void bulk_resize(Image *img, void *params);

#endif // !BULK_H
