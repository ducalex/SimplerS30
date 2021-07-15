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
#include <sys/stat.h>

// Should be .h but this saves a makefile...
#include "cJSON.c"

static const char *basename(const char *path)
{
    const char *name = strrchr(path, '/');
    return name ? name + 1 : path;
}

static int scan_folder(const char *folder, const char *extensions, cJSON *tree)
{
    char buffer1[1024];
    char buffer2[1024];
    int count = 0;
    
    DIR* dir = opendir(folder);
    if (dir)
    {
        fprintf(stderr, "Scanning folder: %s for files of type: %s\n", folder, extensions);
        struct dirent* file;
        
        while ((file = readdir(dir)))
        {
            if (file->d_name[0] == '.')
                continue;
            
            sprintf(buffer1, "%s/%s", folder, file->d_name);

            char *path = realpath(buffer1, buffer2);
            char *name = strcpy(buffer1, file->d_name);
            char *ext = strrchr(name, '.');
            
            if (ext && strstr(extensions, ext + 1))
            {
                name[strlen(name) - strlen(ext)] = 0;
                
                cJSON *item = cJSON_CreateObject();
                cJSON_AddStringToObject(item, "path", path);
                cJSON_AddStringToObject(item, "label", name);
                cJSON_AddStringToObject(item, "core_path", "DETECT");
                cJSON_AddStringToObject(item, "core_name", "DETECT");
                cJSON_AddStringToObject(item, "db_name", "");
                cJSON_AddItemToArray(tree, item);
                count++;
            }
            else
            {
                struct stat path_stat;
                stat(path, &path_stat);
                if (S_ISDIR(path_stat.st_mode))
                {
                    fprintf(stderr, "Recursing into: %s\n", path);
                    count += scan_folder(path, extensions, tree);
                }
            }
        }

        closedir(dir);
    }
    
    return count;
}

int main(int argc, char **argv)
{
    if (argc < 5)
    {
        fprintf(stderr, "Usage: %s playlist_path core_path extensions folder1 [folder2, [...]]\n", argv[0]);
        fprintf(stderr, "Example: %s /path/to/playlist.lpl /path/to/core/snes9x_libretro.so smc,sfc,zip /roms/{sfc,snes}\n", argv[0]);
        return -1;
    }
    
    const char *playlist_path = argv[1];
    const char *core_path = argv[2];
    const char *extensions = argv[3];
    int items_count = 0;
    
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
        items_count += scan_folder(argv[i], extensions, items);
    }
    
    char *str = cJSON_Print(playlist);
    
    if (strcmp(playlist_path, "-") == 0)
    {
        puts(str);
    }
    else if (items_count == 0)
    {
        unlink(playlist_path);
    }
    else
    {
        FILE *fp = fopen(playlist_path, "w");
        if (fp)
        {
            fprintf(stderr, "Writing playlist of %d items to %s\n", items_count, playlist_path);
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
