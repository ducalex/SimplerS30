/**
 * Playlist Generator for RetroArch
 * Author: Alex Duchesne
 * License: GPLv2
 * 
 * $CC playgen.c -o playgen
 */
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// Should be .h but this saves a makefile...
#include "cJSON.c"

static const char *basename(const char *path)
{
    const char *name = strrchr(path, '/');
    return name ? name + 1 : path;
}

static void brace_ext(char *out, const char *in, size_t max_len)
{
    *out++ = '|';
    for (int i = 3; *in && i < max_len; i++)
        *out++ = tolower(*in++);
    *out++ = '|';
    *out++ = 0;
}

static void scan_folder(const char *folder, const char *extensions, cJSON *tree)
{
    char path[PATH_MAX];
    char ext[16];
     
    if (!realpath(folder, path))
        strcpy(path, folder);
    strcat(path, "/");
    
    DIR* dir = opendir(path);
    if (dir)
    {
        fprintf(stderr, "Scanning folder: %s for files of type: %s\n", path, extensions);

        char *filename = path + strlen(path);
        char *ptr = NULL;
        struct dirent* file;

        while ((file = readdir(dir)))
        {
            if (file->d_name[0] == '.')
                continue;

            strcpy(filename, file->d_name);

            if (file->d_type == DT_DIR)
            {
                fprintf(stderr, "Recursing into: %s\n", path);
                scan_folder(path, extensions, tree);
            }
            else if ((ptr = strrchr(filename, '.')))
            {
                brace_ext(ext, ptr + 1, 16);
                if (strstr(extensions, ext))
                {
                    cJSON *item = cJSON_CreateObject();
                    cJSON_AddStringToObject(item, "path", path);
                    *ptr = 0; // Strip extension from path
                    cJSON_AddStringToObject(item, "label", filename);
                    *ptr = '.'; // Add it back in case we need path again
                    cJSON_AddStringToObject(item, "core_path", "DETECT");
                    cJSON_AddStringToObject(item, "core_name", "DETECT");
                    cJSON_AddStringToObject(item, "db_name", "");
                    cJSON_AddItemToArray(tree, item);
                }
            }
        }

        closedir(dir);
    }
}

int main(int argc, char **argv)
{
    if (argc < 5)
    {
        fprintf(stderr, "Usage: %s playlist_path core_path extensions folder1 [folder2, [...]]\n", argv[0]);
        fprintf(stderr, "Example: %s /path/to/playlist.lpl /path/to/core/snes9x_libretro.so smc,sfc,zip /roms/{sfc,snes}\n", argv[0]);
        return -1;
    }
    
    char *playlist_path = argv[1];
    char *core_path = argv[2];
    char extensions[256];

    brace_ext(extensions, argv[3], 256);
    
    cJSON *playlist = cJSON_CreateObject();
    cJSON_AddStringToObject(playlist, "version", "1.4");
    cJSON_AddStringToObject(playlist, "default_core_path", core_path);
    cJSON_AddStringToObject(playlist, "default_core_name", basename(core_path));
    cJSON_AddNumberToObject(playlist, "label_display_mode", 2);
    cJSON_AddNumberToObject(playlist, "right_thumbnail_mode", 0);
    cJSON_AddNumberToObject(playlist, "left_thumbnail_mode", 0);
    cJSON_AddNumberToObject(playlist, "sort_mode", 0);
    cJSON *items = cJSON_AddArrayToObject(playlist, "items");

    for (size_t i = 4; i < argc; i++)
    {
        scan_folder(argv[i], extensions, items);
    }

    int count = cJSON_GetArraySize(items);
    char *str = cJSON_Print(playlist);

    if (strcmp(playlist_path, "-") == 0)
    {
        puts(str);
    }
    else if (count == 0)
    {
        unlink(playlist_path);
    }
    else
    {
        FILE *fp = fopen(playlist_path, "w");
        if (fp)
        {
            fprintf(stderr, "Writing playlist of %d items to %s\n", count, playlist_path);
            fputs(str, fp);
            fclose(fp);
        }
        else
        {
            perror("Unable to open playlist file");
        }
    }
    
    cJSON_free(str);
    cJSON_free(playlist);

    return 0;
}
