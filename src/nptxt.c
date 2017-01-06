/*
	writes np text to text file to be used in IRC as an np script.
    
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
    
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include <deadbeef/deadbeef.h>
#include <stdio.h>
#include <stdlib.h>
#include <config.h>

#ifdef HAVE_LIBGLIB_2_0
#include <glib.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

DB_functions_t *deadbeef;
DB_misc_t plugin;

static int nptxt_songstarted (ddb_event_track_t *ev, uintptr_t data);
static int nptxt_message (uint32_t id, uintptr_t ctx, uint32_t p1, uint32_t p2);
static int song_info (DB_playItem_t *song, float playtime, char *a, char *t, char *b, float *l, char *n);
static int write_nowplaying (int subm, DB_playItem_t *song, time_t started_timestamp);
/**
 * Settings dialog string. Enabled or disabled, filepath /home/x/np.txt. 
 */

static const char settings_dlg[] =
	 "property \"Enable\" checkbox nptxt.enable 0;\n"
	 "property Filepath entry nptxt.filepath \"\";\n"
;

/**
 * Plugin struct info. 
 */
static int nptxt_message (uint32_t id, uintptr_t ctx, uint32_t p1, uint32_t p2) {
    switch (id) {
    case DB_EV_SONGSTARTED:
        nptxt_songstarted ((ddb_event_track_t *)ctx, 0);
        break;
    }
    return 0;
}
DB_misc_t plugin = {
  .plugin.type = DB_PLUGIN_MISC,
  .plugin.api_vmajor = 1,
  .plugin.api_vminor = 0,
  .plugin.version_major = 1,
  .plugin.version_minor = 0,
  .plugin.id = "libnptxt",
  .plugin.name = "NPtxt writer",
  .plugin.descr = "Now Playing Text Writer.\n",
  .plugin.website = "http://eiole",
  .plugin.copyright = 
  "Based on several plugin examples\n. Very plain but works. Be sure to initialize path setting before use,\n"
  "Copyright (C) Rane @ #ylis.fi \n\n"
"This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.\n\n"
"You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.\n"
  ,
  .plugin.configdialog = settings_dlg,
  .plugin.message = nptxt_message,
};

#define META_FIELD_SIZE 200

typedef struct {
    DB_playItem_t *it;
    time_t started_timestamp;
    float playtime;
} subm_item_t;


static int song_info (DB_playItem_t *song, float playtime, char *a, char *t, char *b, float *l, char *n) {
  /*  if (deadbeef->conf_get_int ("nptxt.prefer_album_artist", 0)) {
        if (!deadbeef->pl_get_meta (song, "band", a, META_FIELD_SIZE)) {
            if (!deadbeef->pl_get_meta (song, "album artist", a, META_FIELD_SIZE)) {
                if (!deadbeef->pl_get_meta (song, "albumartist", a, META_FIELD_SIZE)) {
                    if (!deadbeef->pl_get_meta (song, "artist", a, META_FIELD_SIZE)) {
                        return -1;
                    }
                }
            }
        }
    }
    else { */
        if (!deadbeef->pl_get_meta (song, "artist", a, META_FIELD_SIZE)) {
            if (!deadbeef->pl_get_meta (song, "band", a, META_FIELD_SIZE)) {
                if (!deadbeef->pl_get_meta (song, "album artist", a, META_FIELD_SIZE)) {
                    if (!deadbeef->pl_get_meta (song, "albumartist", a, META_FIELD_SIZE)) {
                        return -1;
                    }
                }
            }
        }
    //}
    if (!deadbeef->pl_get_meta (song, "title", t, META_FIELD_SIZE)) {
        return -1;
    }
    if (!deadbeef->pl_get_meta (song, "album", b, META_FIELD_SIZE)) {
        *b = 0;
    }
    *l = deadbeef->pl_get_item_duration (song);
    *l = playtime;

    if (!deadbeef->pl_get_meta (song, "track", n, META_FIELD_SIZE)) {
        *n = 0;
    }
    return 0;
}

static int write_nowplaying (int subm, DB_playItem_t *song, time_t started_timestamp) {
  if(deadbeef->conf_get_int("nptxt.enable", 0)) {
    char a[META_FIELD_SIZE]; // artist
    char t[META_FIELD_SIZE]; // title
    char b[META_FIELD_SIZE]; // album
    float l; // duration
    char n[META_FIELD_SIZE]; // tracknum
    float playtime;
    const char *filepath = deadbeef->conf_get_str_fast ("nptxt.filepath", "");
    if (song_info (song, playtime, a, t, b, &l, n) == 0) {
//       printf ("playtime: %f\nartist: %s\ntitle: %s\nalbum: %s\nduration: %f\ntracknum: %s\n---\n", playtime, a, t, b, l, n);
    }
	if( access( filepath, F_OK ) == -1 ) {
	fprintf (stderr, "NPtxt: File not found or path not writable. Writing to current path/np.txt\n");
	filepath = "np.txt";   // file doesn't exist
	}
	FILE *fp;
	fp=fopen(filepath, "w");
	fprintf(fp, "np: %s [%s #%s] %s\n", a, b, n, t); // Artist [Album #Tracknum] Title
	fclose(fp);
//	printf("filepath: %s out: ", filepath );
}
}


static int nptxt_songstarted (ddb_event_track_t *ev, uintptr_t data) {
//    printf ("nptxt_songstarted %p\n", ev->track);
    if (!deadbeef->conf_get_int ("nptxt.enable", 0)) {
        return 0;
    } 
write_nowplaying (-1, ev->track,ev->started_timestamp);
//printf("Song change detected, write function called.");
return 0;
}

/**
 * Initialize the plugin. 
 */

DB_plugin_t *libnptxt_load (DB_functions_t *ddb) {
  deadbeef = ddb;

#ifdef HAVE_LIBGLIB_2_0
  /* glib main loop */
  GMainLoop *loop;
  loop = g_main_loop_new(NULL,FALSE);
#endif

  return &plugin.plugin;
}
