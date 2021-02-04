//					Bismillahir Rahmanir Raheem
// All praises and gratitudes are due to Allah subhanahu wa Ta'ala
// O Allah: (please do) bless Muhammad (s.a.w) and the Household of Muhammad (s.a.w)

/*************************************************************\
* Muqobil Dasturlar To'plami (c)2009, 2010, 2011, 2012, 2017 *
* 2018, 2019                                                 *
* Purpose:Visual classifier                                  *
* Date:                                                      *
* authors:                                                   *
* original code by TKL K96 207                               *
\*************************************************************/

/* O'zbekiston jumhuriyati, Toshkent shahri, 2008-2017*/
/************************************************************\
 *------------------< TKL 93, 94, 95, 96 >------------------*
 *                   guruhlariga SALOMLAR                   *
\************************************************************/

/************************************************************\
 * Special thanks and greetingz go to Yusuf Erturk CEO of   *
 * MP group www.internetyazilim.com                         *
 *                 Proton Computing Intl.                   *
\************************************************************/
//Please leave this header intact.

#include "ctasnif.h"

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <streambuf>
#include <gdk/gdkkeysyms.h>
#include <glib/gstdio.h>
#include <windows.h>

//#define NORM_CARD_NUM 13

#define usleep(x) Sleep(x)


//GLOBAL Const
const char ver[]={"1.19"};
const char prog_name[]={"GTasnif"};

const char help_wanted[2][280]=
{
"GTasnif 1.15\n\n\
Muqobil Dasturlar To'plami \n\
Copyright (c) 2009-2017 \n\
Released under the terms of GPL3\n\n\
O'zbekiston, Toshkent 1427-1438.\nmuqobildasturlar@gmail.com\n\n\
developed on NetBSD using gcc\n\
with gmake via Geany-1.27\n\n\
       credit due to Yusuf Erturk\n\
       www.netbsd.org",
" F1 -   Кўмак\n \
F2 -   Ўгир\n \
F3 -   Юкла\n \
F6 -   Панжаралар ора кезув\n \
F9 -   Саралаш\n \
F10 - Дастурни тарк этиш\n \
Ctrl ё Alt белгини ўзгартириш"
};

typedef struct
{
  gchar *tag;
  gchar *irbis_data;
  gchar *irbis_field_name;
}
Irbis_Item;

typedef struct
{
  gboolean  fixed;
  gint      file_num;
  gchar    *file_name;
}
File_Item;

enum
{
    COLUMN_FILE_FIXED,
    COLUMN_FILE_NUM,
    COLUMN_FILE_NAME,
    NUM_ITEM_COL
};

enum
{
  COLUMN_ITEM_TAG,
  COLUMN_ITEM_IRBIS_DATA,
  COLUMN_ITEM_IRBIS_FIELD_NAME,
  NUM_ITEM_COLUMNS
};

//GLOBAL vars
static GdkColor text_color, bg_color;

GtkTextBuffer *text_buffer, *mark_vw_buffer;

GtkWidget *auto_inc_check, *precise_hit;
GtkWidget *cards_list, *text_view, *mark_view, *irbis_tree_view;
GtkWidget *irbis_field_list, *marking_done, *font_btn, *mark_check;
GtkWidget *file_tree_view, *auto_put_tags;
GtkWidget *aboutus, *save_all_cards, *load_cards, *change_mark;
GtkWidget *window;//App's Main Window.

bool is_marking = true, is_fixed=false;//Marking process trigger
bool bfocus_in_text = false, bCell_edited = false;
bool bauto_inc = false, is_mark_changed = false, is_precise_hit = true;
bool is_session_restore;
bool is_other_utf_8 = false;
gboolean is_bsorted;
gboolean _fixed;

short g_mark_indx = 0, help_index=0;
char m_mark[5];
char buf[64];
vector<string> files;
int g_file_row;
int gl_width, gl_height, gl_vpaned_pos, gl_hpaned_pos; //window settings
static gint gpos;
gulong kdb_handler_id, row_activated_id;
//char perlc_path[255];//perl interpreter
static GArray *articles = NULL, *files_list = NULL;

//char *pCmd[2];
std::vector< std::string >  pCmd;
int gMarkDir = 0, gFrom_1_file = 0;
gdouble gText_view_x, gText_view_y;

//forwards
//static GtkTreeModel * create_files_list_model (void);
static GtkListStore * create_files_list_model (void);

static void
add_columns_files (GtkTreeView  *treeview,  GtkTreeModel *items_model);

void font_select(GtkFontButton *widget, gpointer       user_data);

void settings();
void RestoreSession();
void SaveSession();
void update_flcursor_location(int row);
void update_files_list_model (void);

//GLOBALS
//CSystem *psys;
CTasnif *psys;

Irbis_Item *foo;
File_Item  *flist;
GtkTreeModel *irbis_items_model;//, *files_list_model;
GtkListStore *files_list_model;
GtkTreeIter tree_iter;
GtkWidget *vpaned, *hpaned;

//session settings
string loadFromPath, save2path, junkFile, app_title;
gchar *font_name;


void fill_list_file_name()
{
  gint indx = 0;
  gchar *entry_text;


  //files_list_model = NULL;
  /* create array */
  g_array_free (files_list, FALSE);
  files_list = g_array_sized_new (FALSE, FALSE, sizeof (File_Item), 1);

  g_return_if_fail ( files_list != NULL);
  //cout << "check if fail" << endl;
  //gtk_tree_view_set_model (GTK_TREE_VIEW (file_tree_view), GTK_TREE_MODEL(files_list_model));

  vector< string >::const_iterator Iter;
  vector< string > _seq = psys->get_cards_file_name();//list of cards file name

  /*ifstream infsession;
  if( is_session_restore )
  {
      infsession.open("session.set");

      if( infsession.fail() ) is_session_restore = false;
      else is_session_restore = true;
  }*/

  for(Iter = _seq.begin(); Iter != _seq.end(); Iter++ )
  {
      indx++;
      //entry_text =  g_convert( (gchar*) Iter->c_str(), -1, "UTF-8", "UTF-8", NULL, NULL, NULL);

      //g_free(flist->file_name);

     flist->file_name =  (gchar*) Iter->c_str();
      //flist->file_name = g_convert( (gchar*) Iter->c_str(), -1, "UTF-8", "UTF-8", NULL, NULL, NULL);

      /*if (is_session_restore )
      {
           infsession >> flist->fixed >> flist->file_num;
           cout << flist->fixed << " " << flist->file_num << endl;
      }
      else*/
      {
           flist->fixed     = FALSE;
           flist->file_num  = indx;
      }

      //flist->file_name = entry_text;
      g_array_append_vals (files_list, flist, 1);

      //g_free(entry_text);
      //cout << entry_text << endl;
  }

  update_files_list_model();
  gtk_widget_grab_focus( GTK_WIDGET( file_tree_view ) );
  /*files_list_model  = create_files_list_model();
  gtk_tree_view_set_model (GTK_TREE_VIEW (file_tree_view), GTK_TREE_MODEL(files_list_model));
  g_object_unref (files_list_model);*/


/*  gtk_clist_clear( GTK_CLIST(cards_list));

  for(Iter = _seq.begin(); Iter != _seq.end(); Iter++ )
  {
    //*Iter += '\n';
    entry_text = (gchar*) Iter->c_str();
    indx = gtk_clist_append( GTK_CLIST(cards_list) , &entry_text);
  }*/

}

void Print_Message( char* _str)
{
    gchar *entry_text;
    GtkTextIter iter, _start, _end;
    gtk_text_buffer_get_start_iter (text_buffer, &_start);
    gtk_text_buffer_get_end_iter (text_buffer, &_end);

    gtk_text_buffer_delete(text_buffer, &_start, &_end);
    gtk_text_buffer_get_iter_at_offset (text_buffer, &iter, 0);

    if(is_other_utf_8 && !g_utf8_validate(_str, -1, NULL))
        entry_text = g_convert( _str, -1, "UTF-8", "CP1251", NULL, NULL, NULL);
    else
        entry_text = g_convert( _str, -1, "UTF-8", "UTF-8", NULL, NULL, NULL);

    gtk_text_buffer_insert (text_buffer, &iter, entry_text , -1);
    g_free (entry_text);
}

gint GlobalInit()
{
  gint indx;

  gchar *pWd = g_get_current_dir();
  junkFile.assign(pWd);
  //junkFile += "\\junk.txt";
  junkFile += "/junk.txt";
  g_free(pWd);

  ifstream fio;
  fio.open("config.set");
  /*pCmd[0] = new char[50];
  pCmd[1] = new char[50];*/

   if( !fio.fail() )
    {
      string _bg_color, _text_color;
      pCmd.resize(2);
          for(short i=0; i < 2; i++)
           {
               //pCmd[i] = new char[128];
               getline(fio, pCmd[i]);


               //g_strstrip(pCmd[i]);
               //cout << i << ": " <<pCmd[i] << endl;
            }
            if( pCmd[0].size() == 0 )
            {
                pCmd[0] = "perl\\not\\found"; //perl path
                pCmd[1] = "script\\not\\found"; // script
            }

           /*fio.getline(perlc_path,100);
           g_strstrip(perlc_path);
           cout << perlc_path << endl;*/
           //fio.getline( &loadFromPath[0], 255 );
           fio >> loadFromPath >> save2path;
           fio >> gl_width >> gl_height >> gl_vpaned_pos >> gl_hpaned_pos;
           fio >> g_file_row;
           fio >> is_session_restore >> gFrom_1_file;

           string buf("Sans Mormal 10");

           fio >> _bg_color >> _text_color;

           if( _bg_color.empty() )
           {
               _bg_color = "#ffffffffffff";
           }

           if(_text_color.empty())
           {
                _text_color = "#000000000000";
           }

           getline(fio, buf);
           getline(fio, buf);
           //fio.getline(&buf[0] , 128);
           font_name = g_strdup( &buf[0]);

           gdk_color_parse ( (gchar*)&_bg_color[0], &bg_color);
           gdk_color_parse ( (gchar*)&_text_color[0], &text_color);


           //cout << save2path << endl;
    }

   /*gtk_text_buffer_create_tag (text_buffer, "blue_foreground",
			      "foreground", "blue", NULL);*/

  indx = 1;
  font_select(NULL, &indx); //restore last font

  //is_session_restore = true;
  /*if( is_session_restore )
  {
     RestoreSession();
     update_flcursor_location(g_file_row);
  }*/

  /*vector< string >::const_iterator Iter;
  vector< string >  _seq = psys->get_irbis_name();//list of irbis fields name

  for(Iter = _seq.begin(); Iter != _seq.end(); Iter++ )
  {
    //*Iter += '\n';
    //entry_text = (gchar*) Iter->c_str();
    entry_text =  g_convert( (gchar*) Iter->c_str(), -1, "UTF-8", "UTF-8", NULL, NULL, NULL);
    indx = gtk_clist_append( GTK_CLIST(irbis_field_list) , &entry_text);
  }*/

  //gtk_clist_sort(GTK_CLIST(clist));//CList sorting

  //GtkTextMark* mmarks =  gtk_text_buffer_create_mark (text_buffer, "#2", NULL, true);

  settings();
  return indx;
}

void GlobalDone(void)
{
   ofstream outf;
   outf.open("config.set");

   //cout << pCmd[0] << " " << pCmd[1] << endl;

   if(pCmd[0].length() != 0)
   {
    outf << pCmd[0] << endl; //perl path
    outf << pCmd[1] << endl; //converter script path
   }

    if(loadFromPath.length() != 0)
    {
        outf << loadFromPath << endl;
        outf << save2path <<endl;
    }


    outf << gl_width << " " << gl_height << " " << gl_vpaned_pos << " " << gl_hpaned_pos <<endl;
    outf << g_file_row << endl;
    outf << is_session_restore << " " << gFrom_1_file << endl;
    outf << gdk_color_to_string(&bg_color) << " " << gdk_color_to_string(&text_color) << endl;
    outf << font_name << endl;

    outf.close();

    if(font_name != NULL)
        g_free(font_name);

    /*for(int i=0; i < 2; i++)
    {
        delete [] pCmd[i];
        //pCmd[i] = NULL;
    }*/

    //is_session_restore = true;
    //if( is_session_restore ) SaveSession();

}

void RestoreSession()
{
  ifstream infsession;

  if( is_session_restore )
  {
      files.clear();

      //files_list_model = NULL;
      /* create array */
      g_array_free (files_list, FALSE);

      files_list = g_array_sized_new (FALSE, FALSE, sizeof (File_Item), 1);

      g_return_if_fail ( files_list != NULL);
      //gtk_tree_view_set_model (GTK_TREE_VIEW (file_tree_view), GTK_TREE_MODEL(files_list_model));

      infsession.open("session.set");
      if( infsession.fail() ) is_session_restore = false;
      else is_session_restore = true;
  }

  int i;
  string filename;

  //infsession >> gFrom_1_file;
  if ( gFrom_1_file == 1 )
  {
     files.push_back(loadFromPath);
     files.push_back(junkFile);
  }
  while(infsession >> flist->fixed >> flist->file_num)
  {
     getline(infsession, filename);

     g_strstrip(&filename[0]);

     //cout << filename <<endl;

     flist->file_name = g_convert( (gchar*) &filename[0], -1, "UTF-8", "UTF-8", NULL, NULL, NULL);

     //cout << flist->file_name << endl;

     if( gFrom_1_file > 1 )
       files.push_back(filename);

     g_array_append_vals (files_list, flist, 1);
  }

  //cout << "From 1 file restore:" << gFrom_1_file << endl;
  infsession.close();
  if (gFrom_1_file > 1 )
       psys->set_cards_files( files );
  //if(gFrom_1_file == 1 )
  else
  {
	   //cout << junkFile << endl;
       //files.push_back(junkFile);
       psys->set_cards_files( files, true );
  }
  if(gFrom_1_file > 1 || gFrom_1_file == 1)
     update_files_list_model();

  /*files_list_model  = create_files_list_model();
  gtk_tree_view_set_model (GTK_TREE_VIEW (file_tree_view), files_list_model);
  g_object_unref (files_list_model);*/

   //fill_list_file_name();
}

void SaveSession()
{

   psys->save_marked_cards(1); //to be continued

   ofstream outf;
   outf.open("session.set");

   //outf <<  gFrom_1_file << endl;

   if( !outf.fail() && files_list->len > 0 )
   {
        for (int i = 0; i < files_list->len; i++)
        {

           outf << g_array_index (files_list, File_Item, i).fixed      <<  " " ;

           outf << g_array_index (files_list, File_Item, i).file_num   << " ";

           outf <<  g_array_index (files_list, File_Item, i).file_name << endl;

        }
    }

   outf.close();
}

int GetNextMark( /*int dir*/)
{
	//gMarkDir = dir;
    int _count = psys->get_marks_num();

    switch(gMarkDir)
     {
       case 0: ++g_mark_indx; break;
       case 1: --g_mark_indx;  break;
     }

     if ( g_mark_indx >  _count-1 )
     {
        //g_mark_indx = 0;
       g_mark_indx = _count-1;
        gMarkDir=1;
     }
     if (g_mark_indx < 0 )
     {
       //g_mark_indx = count-1;
       g_mark_indx = 0;
       gMarkDir=0;
     }

      //cout << gMarkDir << " " <<g_mark_indx << endl;
     strcpy(m_mark, psys->get_marks(g_mark_indx) );


  if (g_mark_indx <= _count )
  {

     GtkTreePath *path = gtk_tree_path_new_from_indices (g_mark_indx, -1);
     gtk_widget_grab_focus (irbis_tree_view);
     gtk_tree_view_set_cursor (GTK_TREE_VIEW (irbis_tree_view),
                                                         path,
                                                         NULL,
                                                         false);
     gtk_tree_path_free (path);
   }

   return _count;
}


void Sync_Mark()
{

    int _count = psys->get_marks_num();
    for(short i = 0; i <= _count; i++)
    {
        if (g_strcmp0( m_mark, psys->get_marks(i) ) == 0 )
        {
            /*cout << "Sync:" << i;
            cout << m_mark << endl;*/
            g_mark_indx = i++;
            is_mark_changed = false;
            break;
        }
    }

}

/*--------------------------callbacks go here------------------------------*/
void mark_toggle( GtkWidget *checkbutton,
                            GtkWidget *entry )
{
 /* gtk_editable_set_editable (GTK_EDITABLE (entry),
                             GTK_TOGGLE_BUTTON (checkbutton)->active);*/
  is_marking = GTK_TOGGLE_BUTTON (checkbutton)->active;
  //is_marking ? cout << "Mark" << endl : cout << "No Mark" << endl;
}

void precise_hit_toggle( GtkWidget *checkbutton,
                            GtkWidget *entry )
{
 /* gtk_editable_set_editable (GTK_EDITABLE (entry),
                             GTK_TOGGLE_BUTTON (checkbutton)->active);*/
  is_precise_hit = GTK_TOGGLE_BUTTON (checkbutton)->active;
  //is_marking ? cout << "Mark" << endl : cout << "No Mark" << endl;
}

void auto_inc_mark_toggle( GtkWidget *checkbutton,
                            GtkWidget *entry )
{
 /* gtk_editable_set_editable (GTK_EDITABLE (entry),
                             GTK_TOGGLE_BUTTON (checkbutton)->active);*/
  bauto_inc = GTK_TOGGLE_BUTTON (checkbutton)->active;
  //is_marking ? cout << "Mark" << endl : cout << "No Mark" << endl;
}

static void
add_irbis_items (void)
{
  //Irbis_Item foo;

  g_return_if_fail (articles != NULL);

  gint indx = 0;
  gchar *entry_text;
  vector< string >::const_iterator Iter;
  vector< string >  _seq = psys->get_irbis_name();//list of irbis fields name

  for(Iter = _seq.begin(); Iter != _seq.end(); Iter++ ,indx++)
  {
    //*Iter += '\n';
    //entry_text = (gchar*) Iter->c_str();
    entry_text =  g_convert( (gchar*) Iter->c_str(), -1, "UTF-8", "UTF-8", NULL, NULL, NULL);
    //indx = gtk_clist_append( GTK_CLIST(irbis_field_list) , &entry_text);
    foo->tag = g_strdup((gchar*)psys->get_marks(indx));
    foo->irbis_data = g_strdup("");
    foo->irbis_field_name = entry_text;
    g_array_append_vals (articles, foo, 1);
  }

}

int update_irbis_fields()
{
  //Irbis_Item foo;
  gboolean is_nxt_iter = TRUE;
  GtkTreeIter iter;
  gint i = 0;


  /*GtkTreePath *path;

  path = gtk_tree_path_new_from_string ("0");
  gtk_tree_model_get_iter ( irbis_items_model,
                           &iter,
                           path);
  gtk_tree_path_free (path);*/

  gtk_tree_model_get_iter_first   (irbis_items_model,  &iter);

  //g_return_if_fail (articles != NULL);
  g_return_val_if_fail(articles != NULL,-1);

  gchar *entry_text, *temp_str;
  gint indx;
  string str;
  vector< string >::const_iterator Iter;
  vector< string >  _seq = psys->get_irbis_data();//list of irbis fields data

  for(Iter = _seq.begin(); Iter != _seq.end(); Iter++, i++ )
  {
    //*Iter += '\n';
    //entry_text = (gchar*) Iter->data();

    /*entry_text =  g_convert( (gchar*) Iter->c_str(), -1, "UTF-8", "UTF-8", NULL, NULL, NULL);
    str.assign((char*)entry_text);*/

    str = *Iter;
    //cout << str <<endl;
    //temp_str = &entry_text[2];
    //temp_str.assign(entry_text);
    //if( str.length() > 1 )

        string::size_type pos=str.find_first_of(' ');
        //cout << "Pos:" << pos << endl;
        if( pos!= basic_string<char>::npos )
        {
            str.erase(0, pos+1);
            //str.copy((char*)temp_str, str.size(), pos+1);
            temp_str = g_convert( (gchar*) str.c_str(), -1, "UTF-8", "UTF-8", NULL, NULL, NULL);
        }
        else
        {
            temp_str = g_strdup("");
        }



        /*str.clear();
        g_free(temp_str);*/

        //temp_str = &(g_strdup( entry_text )[3]);



    //indx = gtk_clist_append( GTK_CLIST(irbis_field_list) , &entry_text);
    //foo.number = 0;
    //foo->irbis_data = entry_text;
    //foo.irbis_field_name = entry_text;


    if( i < articles->len )
    {
            //cout << "i:" << i << " " << temp_str << endl;

                gchar *old_text;

                gtk_tree_model_get (irbis_items_model, &iter, COLUMN_ITEM_IRBIS_DATA, &old_text, -1);
                g_free (old_text);

                g_free (g_array_index (articles, Irbis_Item, i).irbis_data);
                g_array_index (articles, Irbis_Item, i).irbis_data = temp_str;

        //if( is_nxt_iter )

                gtk_list_store_set (GTK_LIST_STORE (irbis_items_model), &iter,
                        COLUMN_ITEM_IRBIS_DATA,
                        g_array_index (articles, Irbis_Item, i).irbis_data,
                        -1);

            is_nxt_iter = gtk_tree_model_iter_next(irbis_items_model, &iter);
        }
  } //end of for


  return i;
}

void get_corrected_irbis_data()
{
   g_return_if_fail (articles != NULL);

   string corr_card;

   for (int i = 0; i < articles->len; i++)
   {
      if( strlen((const char*)g_array_index (articles, Irbis_Item, i).irbis_data) != 0 )
      {
         corr_card += g_array_index (articles, Irbis_Item, i).tag;
         corr_card += ' ';
         corr_card += g_array_index (articles, Irbis_Item, i).irbis_data;
         corr_card += '\n';
      }
   }
   corr_card += "*****\n";
   psys->set_corrected_irbis_card(corr_card);
   //cout << corr_card << endl;
}

/*void get_irbis_fields_data()
{
     /* Get the first iter in the list /
  GtkTreeIter iter;
  gboolean valid=false;
  gint row_count = 0;
  //char *temp_str;
  vector< string >  _seq;
  gchar *irbis_data, *item_tag;

  is_fixed = false;
  //item_tag = g_strdup("");
  //temp_str = new char[512];

  valid = gtk_tree_model_get_iter_first (irbis_items_model, &iter);

  while (valid)
    {
      /* Walk through the list, reading each row */

      /* Make sure you terminate calls to gtk_tree_model_get()
       * with a '-1' value
       /
      gtk_tree_model_get (irbis_items_model, &iter,
                          COLUMN_ITEM_TAG, &item_tag,
                          COLUMN_ITEM_IRBIS_DATA, &irbis_data,
                          -1);

      /* Do something with the data /
      if( strlen((char*)irbis_data)!= 0 )
      {
          //item_tag = g_strconcat  (item_tag, irbis_data);
            strlcpy(buf, (char*)item_tag, sizeof(buf));
            strlcat(buf, (char*)irbis_data, sizeof(buf));
            _seq.push_back(buf);//item_tag);
            //strcat(temp_str, item_tag);
            //cout<< "TEST:"<<row_count<<":" << buf <<endl;
      }
      g_free (irbis_data);
      g_free (item_tag);

      row_count ++;
      valid = gtk_tree_model_iter_next (irbis_items_model, &iter);
    }

    vector< string >::const_iterator Iter;
    for(Iter = _seq.begin(); Iter != _seq.end(); Iter++ )
        {
            cout << "GetIrbisData:"<<Iter->c_str() << endl;
        }

    if ( _seq.size() > 0 ) psys->process(_seq);

    //cout << "TEST:"<<temp_str << endl;
    //delete [] temp_str;
}*/

//static GtkTreeModel *
static GtkListStore *
create_files_list_model (void)
{
  gint i = 0;
  GtkListStore *model;
  GtkTreeIter iter;

  /* create array */
  if( files_list == NULL )
    files_list = g_array_sized_new (FALSE, FALSE, sizeof (File_Item), 1);

  //add_irbis_items ();

  /* create list store */
  model = gtk_list_store_new (NUM_ITEM_COL, G_TYPE_BOOLEAN,
                                G_TYPE_INT, G_TYPE_STRING);

  //cout << "len:"<<files_list->len << endl;
  /* add items */
  for (i = 0; i < files_list->len; i++)
    {
      gtk_list_store_append (model, &iter);

      gtk_list_store_set (model, &iter,
                          COLUMN_FILE_FIXED,
                          g_array_index (files_list, File_Item, i).fixed,
                          COLUMN_FILE_NUM,
                          g_array_index (files_list, File_Item, i).file_num,
                          COLUMN_FILE_NAME,
                          g_array_index (files_list, File_Item, i).file_name,
                          -1);
    }

  //return GTK_TREE_MODEL (model);
  return model;
}


static GtkTreeModel *
create_items_model (void)
{
  gint i = 0;
  GtkListStore *model;
  GtkTreeIter iter;

  /* create array */
  articles = g_array_sized_new (FALSE, FALSE, sizeof (Irbis_Item), 1);

  add_irbis_items ();

  /* create list store */
  model = gtk_list_store_new (NUM_ITEM_COLUMNS, G_TYPE_STRING, G_TYPE_STRING,
                              G_TYPE_STRING);

  /* add items */
  for (i = 0; i < articles->len; i++)
    {
      gtk_list_store_append (model, &iter);

      gtk_list_store_set (model, &iter,
                          COLUMN_ITEM_TAG,
                          g_array_index (articles, Irbis_Item, i).tag,
                          COLUMN_ITEM_IRBIS_FIELD_NAME,
                          g_array_index (articles, Irbis_Item, i).irbis_field_name,
                          COLUMN_ITEM_IRBIS_DATA,
                          g_array_index (articles, Irbis_Item, i).irbis_data,
                          -1);
    }

  return GTK_TREE_MODEL (model);
}


static void
files_selection_changed_cb(GtkTreeSelection *selection, gpointer data)
{
    GtkTreeModel *model;
    gint  row;
    //static gint pos = 0;
    //gboolean is_sorted;
    gchar *entry_text;

    if (gtk_tree_selection_get_selected (selection, &model, &tree_iter))
      {
           gtk_tree_model_get (model, &tree_iter, COLUMN_FILE_FIXED, &is_bsorted, -1);
           gtk_tree_model_get (model, &tree_iter, COLUMN_FILE_NUM, &row, -1);

           //pos == 0 ? pos = (gint)row : pos = pos;
           //g_file_row = (gint)row;

         //cout << "g_file_row:" << ( (int)g_file_row%(int)pos ) <<endl;
         //cout << "pos:" << row <<endl;
         g_file_row = (int)(row - 1);
         //--g_file_row;//normalize file index, because we start countin from -0-
         //cout << "g_file_row:" << (int)g_file_row << " " << psys->get_cards_amount() << endl;

         if(g_file_row == psys->get_cards_amount())
         {
            gtk_widget_set_sensitive(auto_put_tags, FALSE);
            gtk_widget_set_sensitive(save_all_cards, FALSE);
            gtk_widget_set_sensitive(marking_done, FALSE);
         }
         else
         {
             gtk_widget_set_sensitive(save_all_cards, TRUE);
         }

         if( !(is_bsorted) )
          {
            GError *err = NULL;
            //gsize bytes_written = 0;
            //g_print ("You selected a file %d\n", row);

            if(bCell_edited)
            {
               bCell_edited  = false;
               get_corrected_irbis_data();
            }
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (mark_check), TRUE);//enable markin mode

            if(g_file_row == psys->get_cards_amount())
            {
                gtk_widget_set_sensitive(save_all_cards, FALSE);
                gtk_widget_set_sensitive(marking_done, FALSE);
                gtk_widget_set_sensitive(auto_put_tags, FALSE);
            }
            else
            {
                gtk_widget_set_sensitive(marking_done, TRUE);
                gtk_widget_set_sensitive(save_all_cards, TRUE);
                gtk_widget_set_sensitive(auto_put_tags, TRUE);
            }

            //if( is_fixed ) get_irbis_fields_data();

            //const char *pCode_set;
             char *s = psys->load_next_card(g_file_row);
             //if(g_get_charset(&pCode_set))
             entry_text = g_convert( (gchar*) s, -1, "UTF-8", "UTF-8", NULL, NULL, NULL);//&err);
             if ( entry_text == NULL )//err !=  NULL )
             {
               is_other_utf_8  = true;

               //g_free(entry_text);
               //printf("Codeset:%d\n", bytes_written);//err->code);
               //g_error_free(err);

               //entry_text = g_convert( (gchar*) s, -1, "UTF-8", "CP1251", NULL, NULL, NULL);//&err);
               gtk_widget_set_sensitive(auto_put_tags, FALSE);
               gtk_widget_set_sensitive(save_all_cards, FALSE);
               gtk_widget_set_sensitive(marking_done, FALSE);
               Print_Message("\n\n\n\n\n\n\nPlease convert to UTF-8 encoding!");

               //assert( err != NULL );
             }
            else
            {
                is_other_utf_8  = false;
                //gtk_widget_set_sensitive(save_all_cards, TRUE);
                //gtk_widget_set_sensitive(marking_done, TRUE);
            }
              //entry_text = g_locale_to_utf8( (gchar*) s, (gssize)strlen((const char*)s), NULL, NULL, NULL);
            //  entry_text = g_convert( (gchar*) s, (gssize)strlen((const char*)s), "UTF-8", pCode_set, NULL, NULL, NULL);

            //g_print("Character set:%s", pCode_set);

            if( entry_text != NULL )
            {
              GtkTextIter iter, start, end;
              gtk_text_buffer_get_start_iter (text_buffer, &start);
              gtk_text_buffer_get_end_iter (text_buffer, &end);

              gtk_text_buffer_delete(text_buffer, &start, &end);
              gtk_text_buffer_get_iter_at_offset (text_buffer, &iter, 0);

              gtk_text_buffer_insert (text_buffer, &iter,entry_text , -1);
              g_free (entry_text);
            }
         }
         else
         {
            char *s = psys->get_sys_mes();
            string str("\n\n\n\n\n\n\n");
            str.append(s);
            Print_Message(&str[0]);
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (mark_check), FALSE); //disable markin mode
            gtk_widget_set_sensitive(auto_put_tags, FALSE);
         }

         //gtk_widget_set_sensitive(marking_done, !is_sorted);

           //g_free (tag);
      }
}

void update_files_list_model (void)
{
  gint i = 0;
  GtkTreeIter iter;

  /* create array */
  if( files_list == NULL )
    files_list = g_array_sized_new (FALSE, FALSE, sizeof (File_Item), 1);

  //add_irbis_items ();

  gtk_list_store_clear(GTK_LIST_STORE(files_list_model));
  //gtk_tree_store_clear(files_list_model);

  //cout << "len:"<<files_list->len << endl;
  /* add items */
  for (i = 0; i < files_list->len; i++)
    {
      gtk_list_store_append (GTK_LIST_STORE(files_list_model), &iter);

      gtk_list_store_set (GTK_LIST_STORE(files_list_model), &iter,
                          COLUMN_FILE_FIXED,
                          g_array_index (files_list, File_Item, i).fixed,
                          COLUMN_FILE_NUM,
                          g_array_index (files_list, File_Item, i).file_num,
                          COLUMN_FILE_NAME,
                          g_array_index (files_list, File_Item, i).file_name,
                          -1);
    }

}

void update_flcursor_location(int row)
{
    GtkTreeSelection *_select;
    GtkTreePath *path;
    string path_str;

    //memset(path_str, 0, strlen(path_str) );
    g_ascii_dtostr (&path_str[0], 30, row);

    path = gtk_tree_path_new_from_string( path_str.c_str() );

    gtk_tree_selection_select_path (_select, path) ;

    gtk_tree_view_set_cursor (GTK_TREE_VIEW(file_tree_view),  path, NULL, FALSE);

    gtk_tree_view_scroll_to_cell     ( GTK_TREE_VIEW( file_tree_view),
                                                         path,
                                                         NULL,
                                                         FALSE,
                                                         0, 0);

    //g_free( path_str );
    gtk_tree_path_free ( path );
}

static void
tree_selection_changed_cb (GtkTreeSelection *selection, gpointer data)
{
        GtkTreeIter iter;
        GtkTreeModel *model;
        gchar *tag;

        if (gtk_tree_selection_get_selected (selection, &model, &iter))
        {
                gtk_tree_model_get (model, &iter, COLUMN_ITEM_TAG, &tag, -1);

                //g_print ("You selected a tag %s\n", tag);
                strcpy( m_mark, tag);
                is_mark_changed = true;
                gtk_entry_set_text(GTK_ENTRY(mark_view), (const gchar *)m_mark);
                g_free (tag);
        }
}


static void
cell_edited (GtkCellRendererText *cell,
             const gchar         *path_string,
             const gchar         *new_text,
             gpointer             data)
{
  //cout << path_string << endl;
  GtkTreeModel *model = (GtkTreeModel *)data;
  GtkTreePath *path = gtk_tree_path_new_from_string (path_string);
  GtkTreeIter iter;

  gint column = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (cell), "column"));

  gtk_tree_model_get_iter (model, &iter, path);
  is_fixed = true;

  switch (column)
    {
    /*case COLUMN_ITEM_TAG:
      {
        gint i;

        i = gtk_tree_path_get_indices (path)[0];
        g_array_index (articles, Irbis_Item, i).tag = atoi (new_text);

        gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
                            g_array_index (articles, Irbis_Item, i).number, -1);
      }
      break;*/

    case COLUMN_ITEM_IRBIS_DATA:
      {
        gint i;
        gchar *old_text;

        bCell_edited = true;
        gtk_tree_model_get (model, &iter, column, &old_text, -1);
        g_free (old_text);

        i = gtk_tree_path_get_indices (path)[0];
        g_free (g_array_index (articles, Irbis_Item, i).irbis_data);
        g_array_index (articles, Irbis_Item, i).irbis_data = g_strdup (new_text);

        gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
                            g_array_index (articles, Irbis_Item, i).irbis_data, -1);
      }
      break;
    }

  gtk_tree_path_free (path);
}

static void
add_columns (GtkTreeView  *treeview,
             GtkTreeModel *items_model
             /*GtkTreeModel *numbers_model*/)
{
  GtkCellRenderer *renderer;

 /* yummy column */
  //renderer = gtk_cell_renderer_progress_new ();
  renderer = gtk_cell_renderer_text_new ();
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (COLUMN_ITEM_TAG));

  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
                                               -1, "Tags", renderer,
                                               "text", COLUMN_ITEM_TAG,
                                               NULL);


  /* IRBIS field name column */
  renderer = gtk_cell_renderer_text_new ();
  /*g_object_set (renderer,
                "editable", TRUE,
                NULL);*/
  g_signal_connect (renderer, "edited",
                    G_CALLBACK (cell_edited), items_model);
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (COLUMN_ITEM_IRBIS_FIELD_NAME));
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
                                               -1, "Fields name", renderer,
                                               "text", COLUMN_ITEM_IRBIS_FIELD_NAME,
                                               NULL);


  /* irbis data column */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (renderer,
                "editable", TRUE,
                NULL);
  g_signal_connect (renderer, "edited",
                    G_CALLBACK (cell_edited), items_model);
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (COLUMN_ITEM_IRBIS_DATA));

  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
                                               -1, "Data", renderer,
                                               "text", COLUMN_ITEM_IRBIS_DATA,
                                               NULL);


}

static void
fixed_toggled (GtkCellRendererToggle *cell,
	       gchar                 *path_str,
	       gpointer               data)
{
  GtkTreeModel *model = (GtkTreeModel *)data;
  gint row, *column;
  GtkTreeIter  iter;
  //cout << path_str << " " << g_file_row << endl;
  GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
  //gboolean _fixed;

  //column = (gint*)g_object_get_data (G_OBJECT (cell), "column");

  /* get toggled iter */
  gtk_tree_model_get_iter (GTK_TREE_MODEL(files_list_model), &iter, path);
  gtk_tree_model_get (GTK_TREE_MODEL(files_list_model), &iter, COLUMN_FILE_FIXED, &_fixed, -1);
  //gtk_tree_model_get (model, &tree_iter, COLUMN_FILE_FIXED, &_fixed, -1);

  /* do something with the value */
  _fixed ^= 1;
  //_fixed = cell->active;//gtk_cell_renderer_toggle_get_active(cell);

  gtk_widget_set_sensitive(marking_done,  !_fixed);
  gtk_widget_set_sensitive(auto_put_tags, !_fixed);

  gtk_tree_model_get (model, &tree_iter, COLUMN_FILE_NUM, &row, -1);

  g_file_row = (int)(row);

  //g_file_row = GPOINTER_TO_INT(row);
  --g_file_row;//normalize file index
  //cout << g_file_row << endl;

  char *s;
  _fixed ? s = psys->get_sys_mes() : s = psys->load_next_card(g_file_row);
  //Print_Message(s);

 if(is_other_utf_8 && !g_utf8_validate(s, -1, NULL))
 {
     gtk_widget_set_sensitive(marking_done,  FALSE);
     gtk_widget_set_sensitive(auto_put_tags, FALSE);
     gtk_widget_set_sensitive(save_all_cards, FALSE);
     Print_Message("\n\n\n\n\n\n\nPlease convert to UTF-8 encoding!");
 }
 else
 {
    if(_fixed )
    {
        string str("\n\n\n\n\n\n\n");
        str.append(s);
        Print_Message(&str[0]);
    }
    else
        Print_Message(s);

 }


  /* set new value */
  gtk_list_store_set (GTK_LIST_STORE (model), &iter, COLUMN_FILE_FIXED, _fixed, -1);
  //gtk_cell_renderer_toggle_set_active(cell, _fixed);
  //cell->active = _fixed;
  g_array_index (files_list, File_Item, g_file_row).fixed  = _fixed;

  /* clean up */
  gtk_tree_path_free (path);
}

static void
add_columns_files (GtkTreeView  *treeview,
             GtkTreeModel *items_model
             /*GtkTreeModel *numbers_model*/)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  //GtkTreeModel *model = gtk_tree_view_get_model (treeview);

  /* column for fixed toggles */
  renderer = gtk_cell_renderer_toggle_new ();
  //g_object_set_data (G_OBJECT (renderer), "column", (gint *)COLUMN_FILE_FIXED);

  g_signal_connect (renderer, "toggled",
		    G_CALLBACK (fixed_toggled), files_list_model);

  column = gtk_tree_view_column_new_with_attributes ("",
						     renderer,
						     "active", COLUMN_FILE_FIXED,
						     NULL);

  /* set this column to a fixed sizing (of 50 pixels) */
  gtk_tree_view_column_set_sizing (GTK_TREE_VIEW_COLUMN (column),
				   GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_fixed_width (GTK_TREE_VIEW_COLUMN (column), 50);
  //gtk_tree_view_column_set_clickable (GTK_TREE_VIEW_COLUMN (column), TRUE);
  gtk_tree_view_append_column (treeview, column);


  /* column for file numbers */
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Tartibi",
						     renderer,
						     "text",
						     COLUMN_FILE_NUM,
						     NULL);
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_FILE_NUM);
  gtk_tree_view_append_column (treeview, column);

   /* column for file names */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (COLUMN_FILE_NAME));

  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
                                               -1, "Nomi", renderer,
                                               "text", COLUMN_FILE_NAME,
                                               NULL);

}

static void load_cards_event( GtkWidget      *widget,
                                    gpointer   data )
{

// gboolean select_multiple;
 GtkWidget *dialog;
 char *filename;
 GSList *filenames;

 files.clear();//list of files
 is_session_restore = false;

 dialog = gtk_file_chooser_dialog_new ("Open File",
				      NULL,
				      GTK_FILE_CHOOSER_ACTION_OPEN,
				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
				      NULL);

 gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER (dialog), TRUE);

  //cout << loadFromPath << endl;

  if( loadFromPath.size() > 0 )
  {
        gchar *path;
        path = g_path_get_dirname( (const gchar *)&loadFromPath[0] );
        //cout << path << endl;

        gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog),(const gchar *)path);
        /*gtk_file_chooser_select_filename    (GTK_FILE_CHOOSER (dialog),
                                                            (const char *)path);*/

        g_free(path);
    }


 if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
   {

     //filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
     filenames = gtk_file_chooser_get_filenames (GTK_FILE_CHOOSER (dialog));
    //open_file (filename);
    // g_print("%s", (gchar*)g_slist_nth_data (filenames, 1));

      guint num = g_slist_length(filenames);
      gFrom_1_file = (int)num;
      //cout << num << endl;
        for( guint i=0; i < num; i++)
        {
          filename = (char*)g_slist_nth_data (filenames, i);
          //g_print("%s\n", (gchar*)filename );
          files.push_back(filename);
          g_free (filename);
        }
      if(num > 1)
      {
       psys->set_cards_files( files );


       //GlobalInit();

       //gtk_widget_set_sensitive(marking_done, TRUE);
      }
      else
      {
          //cout << "junk:" << junkFile << endl;
          if(psys->is_1_card(files))
          {
              psys->set_cards_files( files );
          }
          else
          {
            files.push_back(junkFile);
            psys->set_cards_files( files, true );
          }
      }
      loadFromPath = files[0];
      //cout << "loadfrompath:" << files[0] << endl;
      fill_list_file_name();
      g_slist_free(filenames);
   }


 /*select_multiple = gtk_file_chooser_get_select_multiple(GTK_FILE_CHOOSER (dialog));
 if( select_multiple ) g_print("TRUE"); */

    app_title = loadFromPath;
    app_title +=" - ";
    app_title.append(buf);
    gtk_window_set_title(GTK_WINDOW(window), app_title.c_str());
    gtk_widget_destroy (dialog);
}

/*static void mark_release_event( GtkWidget      *widget,
                                GdkEvent       *event,
                                gpointer        data )
{
    cout << "mark release" << endl;
}*/

/*static void mark_pressed_event( GtkWidget      *widget,
                                GdkEvent       *event,
                                gpointer        data )
{
   if(is_mark_changed) Sync_Mark();
   GetNextMark();//static_cast<int>(*pDir));
   gtk_entry_set_text(GTK_ENTRY(mark_view), (const gchar *)m_mark);
}*/

static void mark_change_event( GtkWidget      *widget,
                                    gpointer   data )
{
    //unsigned char* pDir = reinterpret_cast<unsigned char*>(data);

   if(is_mark_changed) Sync_Mark();
   GetNextMark();//static_cast<int>(*pDir));
   gtk_entry_set_text(GTK_ENTRY(mark_view), (const gchar *)m_mark);

}

static void mark_done_event( GtkWidget      *widget,
                                    gpointer   data )
{

  GtkTextIter _start, _end;
  gtk_text_buffer_get_start_iter (text_buffer, &_start);
  gtk_text_buffer_get_end_iter (text_buffer, &_end);

  gchar* str = gtk_text_buffer_get_text (text_buffer,&_start,&_end, false);

  psys->process((char*)str); //get tokens according tags & fill isis_fields table
  update_irbis_fields();
  g_free(str);

    gint *row;
  //gboolean is_sorted;

  gtk_tree_model_get (GTK_TREE_MODEL(files_list_model), &tree_iter, COLUMN_FILE_FIXED, &is_bsorted, -1);

  is_bsorted ^=1; //put tick mark for processed file
  //is_marking = false;
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (mark_check), FALSE);

  gtk_list_store_set (GTK_LIST_STORE (files_list_model), &tree_iter, COLUMN_FILE_FIXED,
                        is_bsorted, -1);

  g_array_index (files_list, File_Item, g_file_row).fixed  = is_bsorted;

  gtk_widget_set_sensitive(marking_done,  !is_bsorted);
  gtk_widget_set_sensitive(auto_put_tags, !is_bsorted);

  if (is_bsorted)
  {
      char *s = psys->get_sys_mes();
      string str("\n\n\n\n\n\n\n");
      str.append(s);
      Print_Message(&str[0]);
  }

  if (!is_session_restore)
  {
      fstream ftmp("temp.txt", ios::out);
      ftmp.close();
  }

  is_session_restore = true; //save the session
  //cout << str <<endl;
  //printf("%s", str);
}

static void auto_put_tags_event( GtkWidget      *widget,
                                    gpointer   data )
{
    bool no_need_2_delete = true;
    const char file_name[] = "theAutoPutTags.txt";
    fstream ftmp;
    //ftmp.open(file_name, ios::out | ios::trunc);
    //ftmp.close();

    ftmp.open(file_name, ios::out);

    if( !ftmp.fail() )
    {
        string cmd;
        gchar *entry_text;
        gchar *curDirPath;
        gchar* _str = NULL;
        char *_Matn = NULL;

        GtkTextIter _start, _end;


        if(is_marking)
        {
            _Matn = psys->load_next_card(g_file_row); //get_card_text();
        }
        else
        {
            gtk_text_buffer_get_start_iter (text_buffer, &_start);
            gtk_text_buffer_get_end_iter (text_buffer, &_end);

            //Print_Message( psys->load_next_card(g_file_row));
            _str = gtk_text_buffer_get_text (text_buffer,&_start,&_end, true);
            //gtk_text_buffer_delete(text_buffer, &_start, &_end);

            if(_str != NULL)
            {
                //_Matn = new char[255];
                //_Matn = psys->get_card_text();
                //memset( _Matn, 0, strlen(_Matn) ); //clean it up, to be empty
                //if(!memcpy( _Matn, (char*)_str, strlen(_str) ))
                if(psys->set_card_text((char*)_str))
                {
                    _Matn = psys->get_card_text();
                    no_need_2_delete = false;
                    g_free(_str);
                }
            }

        }


        if( _Matn != NULL )
        {
            if(is_other_utf_8 && !g_utf8_validate(_Matn, -1, NULL))
            //    entry_text = g_convert( (gchar*) _Matn, -1, "UTF-8", "CP1251", NULL, NULL, NULL) ;
                  Print_Message("\n\n\n\n\n\n\nPlease convert to UTF-8 encoding!");
            else
                entry_text = g_convert( (gchar*) _Matn, -1, "UTF-8", "UTF-8", NULL, NULL, NULL) ;
        }


        if( entry_text != NULL )
        {
            curDirPath = g_get_current_dir();
            //ftmp << tmpFilePath << "\\" << "\n";
            //ftmp << _Matn;
            ftmp << entry_text;

            g_free(entry_text);

            ftmp.close();

            if(!ftmp.fail())
            {

                cmd  =  &(pCmd[0])[0]; //perl path
                cmd += " ";
                cmd += curDirPath;
                cmd += "\\autoPutTags.pl ";
                cmd += curDirPath;
                cmd += "\\theAutoPutTags.txt ";
                cmd += curDirPath;
                cmd += "\\outPutFile.txt";

                //ftmp << &cmd[0];

                system( (char*)&cmd[0]);

                //usleep(250);

                cmd = curDirPath;
                cmd += "\\outPutFile.txt";
                //ftmp.open(cmd, ios::in | ios::trunc);
                //ftmp.close();
                ftmp.open(cmd, ios_base::in);

                if(ftmp.is_open())
                {
                    string str;
                    memset( _Matn, 0, strlen(_Matn) );//universal
                    while( !ftmp.eof() )
                    {
                        getline(ftmp, str);
                        str += "\n";
                        strcat(_Matn, &str[0]);
                        //cout << _Matn;
                    }
                   Print_Message(_Matn);
                }
                ftmp.close();
            }

            g_free(curDirPath);
            //if(!no_need_2_delete)
            {
                //delete [] _Matn;
                //g_free(_str);
            }

            if( !GTK_WIDGET_IS_SENSITIVE(marking_done) )
                gtk_widget_set_sensitive(marking_done, TRUE);
        }

    }
    //ftmp.close();
}

static void save_all_cards_event( GtkWidget      *widget,
                                    gpointer   data )
{
    string str;
    time_t rawTime;
	char buf_tm[20];
	//struct timeval timeNow;
	struct tm *_pTime;

    is_session_restore ? psys->save_marked_cards(2) : psys->save_marked_cards(0); //to be concluded or oneshot

    GtkWidget *dialog;

    dialog = gtk_file_chooser_dialog_new ("Save File",
				      NULL,//parent_window,
				      GTK_FILE_CHOOSER_ACTION_SAVE,
				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
				      NULL);
    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

    //cout << save2path << endl;
    gchar *path = NULL;
    if ( save2path.size() > 0 ) {

        //gchar *path;
        path = g_path_get_dirname( (const gchar *)&save2path[0] );

        //cout << save2path << endl;

        gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog),    (const gchar *)path );
        //g_free( path );
    }

    /*if (user_edited_a_new_document)
    {
        gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), default_folder_for_saving);
        gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), "Untitled document");
    }
    else
    gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (dialog), filename_for_existing_document);*/


    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        gchar *tmpFilePath;
        //gchar *cmd;
        string cmd;

        time( &rawTime );
        _pTime = localtime( &rawTime );

        strftime(buf_tm, sizeof(buf_tm),"%Y.%m.%d_%H%M%S", _pTime);

        tmpFilePath = g_get_current_dir();
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        //save_to_file (filename);
        //cmd = perlc_path;
        //cmd = pCmd[0];

        //strcpy( cmd, pCmd[0]);
        //strcat(cmd, " ");

        //strcat(cmd, " card2isis.pl temp.txt ");
        /*strcat(pCmd[1],  " temp.txt ");
        strcat(cmd, pCmd[1]);*/

        //cmd = g_strdup("card2isis.pl temp.txt ");
        //cmd = g_strconcat( cmd, (gchar*)filename);

        //strcat( cmd, filename);

        cmd  =  &(pCmd[0])[0]; //perl path
        cmd += " ";
        cmd += &(pCmd[1])[0]; //card2isis.pl
        //cmd += " temp.txt ";
        cmd += " ";
        cmd +=tmpFilePath;
        cmd += "\\temp.txt ";
        cmd += filename;

        //cout << cmd << endl;
        system( (char*)&cmd[0] );

        save2path.assign( filename );

        g_free (filename);
        g_free(tmpFilePath);
        //g_free(cmd);
    }

    is_session_restore = false; //session done, start from new one

    //sstream sstr;
    //string backup_name_path(&save2path[0]);

    str.assign(buf_tm);
    str += ".txt";
    gchar *backup_path = g_build_filename(path, &str[0], NULL);

    //backup_name_path +="/" ;

    //rename("temp.txt", backup_path);
    g_rename("temp.txt" ,backup_path);

    //cout << backup_path << endl;

    g_free( backup_path);
    g_free( path );

    //clean temp.txt file up
    fstream ftmp("temp.txt", ios::out)/*, ores(&loadFromPath[0], ios::out)*/;
    /*ftmp.flush();*/
    ftmp.close();


    gtk_widget_destroy (dialog);
}

static gboolean mark_hit_event( GtkWidget      *widget,
                                    GdkEventButton *event )
{
   //char m_mark[3];

   if ( event->button == 1 )
   {
       if(is_mark_changed) Sync_Mark();
      GetNextMark();
      //strcpy(m_mark, psys->get_marks(g_mark_indx) );
      //printf("%s", psys->get_marks(g_mark_indx) );

/*     GtkTextIter iter, start, end;
       gtk_text_buffer_get_start_iter (mark_vw_buffer, &start);
       gtk_text_buffer_get_end_iter (mark_vw_buffer, &end);

       gtk_text_buffer_delete(mark_vw_buffer, &start, &end);
       gtk_text_buffer_get_iter_at_offset (mark_vw_buffer, &iter, 0);*/
     //gtk_text_buffer_insert (mark_vw_buffer, &iter, "#", -1);
     //(const gchar*)m_mark
      //gtk_text_buffer_set_text (mark_vw_buffer, "#", -1);

     gtk_entry_set_text(GTK_ENTRY(mark_view), (const gchar *)m_mark);
     /*++g_mark_indx;
     if( g_mark_indx > 4 ) g_mark_indx = 0;*/
   }
   return TRUE;
}

gboolean   check_for_ws(gunichar ch,    gpointer _counter)
{
    gboolean bOK = FALSE;
    gint *offset = (gint*)(_counter);

    //++(*offset);

    if(ch == GDK_space  || ch == 0x09 || ch == 0x0A || ch == 0x0D || ch == GDK_comma ||
       ch == GDK_period || ch == GDK_colon || ch == GDK_semicolon)
       {
            bOK = TRUE;
            ++(*offset);
       }

    // ++(*offset);
    //cout << *offset << " " << hex <<  ch << endl;

    /*if( (ch >= GDK_a &&  ch <= GDK_z)  ||  (ch >= GDK_A &&  ch <= GDK_Z) )
        bOK = FALSE;*/

    return bOK;
}


static gboolean focus_out_event( GtkWidget      *widget,
                                    GdkEvent      *event,
                                    gpointer       data)
{
   //cout << "text buffer view" << endl;
   bfocus_in_text = false;
   return false;
}

static gboolean focus_in_event( GtkWidget      *widget,
                                    GdkEvent      *event,
                                    gpointer       data)
{
   //cout << "text buffer view" << endl;
   bfocus_in_text = true;
   return false;
}

static gboolean enter_notify_event( GtkWidget      *widget,
                                    GdkEvent      *event,
                                    gpointer       data)
{
   gdouble x, y;
   if(event->type == GDK_MOTION_NOTIFY)
   {
	     bfocus_in_text = true;
         //cout << "text buffer view" << endl;
         gdk_event_get_coords (event, &gText_view_x, &gText_view_y);
         //cout << gText_view_x << " " << gText_view_y << endl;
    }
    if (event->type == GDK_LEAVE_NOTIFY)
                    bfocus_in_text = false;
   /*if (gdk_event_get_coords (event, &x, &y))
    {
      cout << x << " " << y << endl;
    }*/
   return true;
}

static gboolean button_press_event( GtkWidget      *widget,
                                    GdkEventButton *event )
{
  //cout << "test btn" << endl;
  //gunichar is_sharp;
  //static short indx = g_mark_indx;
  GtkTextIter iter;
  gint buffer_x, buffer_y, offset_char;
  gint counter = 0;
  //char * m_mark;
  short count = psys->get_marks_num();

  if ( (event->button == 1 ) && (is_marking) )
  {
       gtk_text_view_window_to_buffer_coords(GTK_TEXT_VIEW(widget),
                                             GTK_TEXT_WINDOW_WIDGET,
                                                       (gint)event->x,
                                                       (gint)event->y,
                                                       &buffer_x,
                                                       &buffer_y);

       gtk_text_view_get_iter_at_location  (GTK_TEXT_VIEW(widget),
                                                         &iter,
                                                         buffer_x, buffer_y);

      if( !is_precise_hit  && g_mark_indx < count-1)
            gtk_text_iter_backward_find_char (&iter,  check_for_ws,  &counter, NULL);
      //is_sharp = gtk_text_iter_get_char(dynamic_cast<const GtkTextIter *>(&iter));
      gtk_text_iter_forward_chars(&iter, counter);

      gtk_text_buffer_place_cursor (text_buffer,  dynamic_cast<const GtkTextIter *>(&iter));

      /*if(is_sharp == '#')
        {
           //cout << "gotcha" << endl;
           offset_char = gtk_text_iter_get_offset(dynamic_cast<const GtkTextIter *>(&iter));
           end = iter;
           offset_char+=2;
           gtk_text_iter_set_line_offset(&end, offset_char);

           gtk_text_buffer_delete(text_buffer, &iter, &end);
           //gtk_text_buffer_backspace(buffer, &end, false, true);
//           gtk_text_buffer_set_text (buffer, "#1", 2);
           return true;
        }*/
      //else
      {
       //strcpy(m_mark, psys->get_marks(g_mark_indx) );
       //GetNextMark();
       gtk_text_buffer_insert_with_tags_by_name (text_buffer, &iter,
					    (const gchar*)m_mark, -1,
					    "blue_foreground", NULL);
       //gtk_text_buffer_insert_at_cursor(text_buffer, (const gchar*)m_mark, -1);
        if( !GTK_WIDGET_IS_SENSITIVE(marking_done) )
            gtk_widget_set_sensitive(marking_done, TRUE);
      }

      if( bauto_inc )
      {
        if(is_mark_changed) Sync_Mark();
        GetNextMark();
        gtk_entry_set_text(GTK_ENTRY(mark_view), (const gchar *)m_mark);
      }

      //cout << ++g_mark_indx << endl;
      /*++g_mark_indx;
      if( g_mark_indx > 4 ) g_mark_indx = 0;*/
    return TRUE;
  }

  if( (event->button == 3 ) && (is_marking) )
  {
      if(is_mark_changed) Sync_Mark();
      GetNextMark();
      gtk_entry_set_text(GTK_ENTRY(mark_view), (const gchar *)m_mark);
      return TRUE;
  }
   //cout << buffer_x << ":"<< buffer_y <<endl;


  return FALSE;
}

void selection_made1 (GtkTreeView       *tree_view,
                     GtkTreePath       *path,
                     GtkTreeViewColumn *column,
                     gpointer           user_data)
{
    GtkTreeIter tree_iter;
    gint *row;


    //gtk_tree_model_get_iter (files_list_model, &tree_iter, path);
    //gtk_tree_model_get (files_list_model, &tree_iter, COLUMN_FILE_NUM, &row, -1);
    //cout << *row<<endl;

}

/* If we come here, then the user has selected a row in the list. */
void selection_made( GtkWidget      *clist,
                     gint            row,
                     gint            column,
                     GdkEventButton *event,
                     gpointer        data )
{
    gchar *entry_text;
    //if( is_fixed ) get_irbis_fields_data();

    /* Get the text that is stored in the selected row and column
     * which was clicked in. We will receive it as a pointer in the
     * argument text.
     */
    //gtk_clist_get_text(GTK_CLIST(clist), row, 0, &entry_text);

  /* Just prints some information about the selected row */
  /* g_print("You selected row %d. More specifically you clicked in "
            "column %d, and the text in this cell is %s\n\n",
            row, column, entry_text);*/


  char *s = psys->load_next_card(row);
  entry_text = g_convert( (gchar*) s, -1, "UTF-8", "UTF-8", NULL, NULL, NULL);

  //g_print("%s", entry_text);

  //gtk_text_view_set_editable( GTK_TEXT_VIEW ( text_view ), false);

  GtkTextIter iter, start, end;
  gtk_text_buffer_get_start_iter (text_buffer, &start);
  gtk_text_buffer_get_end_iter (text_buffer, &end);

  gtk_text_buffer_delete(text_buffer, &start, &end);
  gtk_text_buffer_get_iter_at_offset (text_buffer, &iter, 0);

  //gtk_text_buffer_set_text (text_buffer, " ", -1);
  gtk_text_buffer_insert (text_buffer, &iter, entry_text , -1);
  g_free (entry_text);

}


void font_select(GtkFontButton *widget,
                  gpointer       user_data)
{
  //gchar *font_name;

  int *sig = (int*)(user_data);
  if( *sig != 1)
  {
    if(font_name != NULL) g_free(font_name);
    font_name = g_strdup(gtk_font_button_get_font_name(GTK_FONT_BUTTON(font_btn)) );

    //if(strlen(font_name) )
  }

  //cout << font_name << endl;
  gtk_font_button_set_font_name(GTK_FONT_BUTTON(font_btn), font_name);

  PangoFontDescription *font_desc;
 /* Change default font throughout the widget */
  font_desc = pango_font_description_from_string (font_name);
  gtk_widget_modify_font (text_view, font_desc);
  pango_font_description_free (font_desc);

  //g_free(font_name);
}

static void
color_select_cb (GtkWidget *button,
                    gpointer	  data)
{
  GtkWidget *dialog;
  GtkColorSelection *colorsel;
  gint response;
  gint i = GPOINTER_TO_INT(data); //which part of trans_text_view change color

  dialog = gtk_color_selection_dialog_new ("Changing color");

  gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (window));

  colorsel = GTK_COLOR_SELECTION (GTK_COLOR_SELECTION_DIALOG (dialog)->colorsel);

  //continue from last selected color: 0 -> text, 1->background; relativist ultrix
  gtk_color_selection_set_previous_color (colorsel, i > 0 ? &bg_color : &text_color);
  gtk_color_selection_set_current_color (colorsel,  i > 0 ? &bg_color : &text_color);
  gtk_color_selection_set_has_palette (colorsel, TRUE);

  response = gtk_dialog_run (GTK_DIALOG (dialog));

  if (response == GTK_RESPONSE_OK)
    {

      switch(i)
      {
            case 0: gtk_color_selection_get_current_color (colorsel,
                                                            &text_color); break;

            case 1: gtk_color_selection_get_current_color (colorsel,
                                                            &bg_color);break;
       }

    }

  gtk_widget_destroy (dialog);
}

//settings func
void settings()
{
    PangoFontDescription *font_desc;
    /* Change default font throughout the widget */
    font_desc = pango_font_description_from_string (font_name);

    gtk_widget_modify_font ( text_view, font_desc);

    pango_font_description_free (font_desc);


    /*if ( !GTK_WIDGET_HAS_FOCUS (words_tree_view) )
    {

        gtk_widget_grab_focus( GTK_WIDGET( words_tree_view));

    }*/

    gtk_widget_modify_text (text_view, GTK_STATE_NORMAL, &text_color);
    gtk_widget_modify_base (text_view, GTK_STATE_NORMAL, &bg_color);
}

//config dialog
static void
conf_dialog_clicked (GtkButton *btn,
			    gpointer   user_data)
{
  GtkWidget *dialog;
  GtkWidget *hbox, *button;
  GtkWidget *stock;
  GtkWidget *table;
  GtkWidget *edcheck, *auto_search;
  //GtkWidget *local_entry2;
  GtkWidget *label;
  gint response, text_or_bg;

  dialog = gtk_dialog_new_with_buttons("Settings:",
					(GtkWindow*)user_data,//GTK_WINDOW (window),
					(GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
					GTK_STOCK_OK,
					GTK_RESPONSE_OK,
					"_Cancel",
					GTK_RESPONSE_CANCEL,
					NULL);

  hbox = gtk_hbox_new (FALSE, 8);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 8);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->vbox), hbox, FALSE, FALSE, 0);

  stock = gtk_image_new_from_stock (GTK_STOCK_DIALOG_QUESTION, GTK_ICON_SIZE_DIALOG);
  gtk_box_pack_start (GTK_BOX (hbox), stock, FALSE, FALSE, 0);

  table = gtk_table_new (2, 2, FALSE);
  gtk_table_set_row_spacings (GTK_TABLE (table), 4);
  gtk_table_set_col_spacings (GTK_TABLE (table), 4);
  gtk_box_pack_start (GTK_BOX (hbox), table, TRUE, TRUE, 0);

/*----------------------------------------font selection--------------------------------------*/
  label = gtk_label_new ("Font:");
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
  font_btn = gtk_font_button_new ();

  gtk_font_button_set_font_name(GTK_FONT_BUTTON(font_btn),   font_name);
  gtk_table_attach_defaults (GTK_TABLE (table), label,    0, 1, 0, 1);
  gtk_table_attach_defaults (GTK_TABLE (table), font_btn, 1, 2, 0, 1);
  g_signal_connect(GTK_OBJECT(font_btn), "font-set",
                        GTK_SIGNAL_FUNC(font_select), window);
/*-------------------------------------end of font selection--------------------------------------*/

/*----------------------------------------color selection--------------------------------------*/
  text_or_bg = 0;
  label = gtk_label_new ("Text color:");
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
  button = gtk_button_new_with_mnemonic ("_Text color");

  gtk_table_attach_defaults (GTK_TABLE (table), label,    0, 1, 1, 2);
  gtk_table_attach_defaults (GTK_TABLE (table), button, 1, 2, 1, 2);
  g_signal_connect(GTK_OBJECT(button), "clicked",
                        G_CALLBACK(color_select_cb), GINT_TO_POINTER(text_or_bg));
/*-------------------------------------end of color selection------------------------------------*/

/*----------------------------------------bg color selection--------------------------------------*/
  text_or_bg = 1;
  label = gtk_label_new ("Background color:");
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
  button = gtk_button_new_with_mnemonic ("_Background color");

  gtk_table_attach_defaults (GTK_TABLE (table), label,    0, 1, 2, 3);
  gtk_table_attach_defaults (GTK_TABLE (table), button, 1, 2, 2, 3);
  g_signal_connect(GTK_OBJECT(button), "clicked",
                        G_CALLBACK(color_select_cb),  GINT_TO_POINTER(text_or_bg));
/*-------------------------------------end of bg color selection-----------------------------------*/


  gtk_widget_show_all (hbox);
  response = gtk_dialog_run (GTK_DIALOG (dialog));

  if (response == GTK_RESPONSE_OK)
    {
      settings();

    }
    else if(response == GTK_RESPONSE_CANCEL )
    {
        //_auto_search = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( auto_search ) );
    }

  gtk_widget_destroy (dialog);
}

#define ALL_ACCELS_MASK (GDK_CONTROL_MASK, GDK_SHIFT_MASK, GDK_MOD1_MASK)
//GValue gVal = G_VALUE_INIT;
GtkTreeSelection *selection, *sel;
//GdkEventKey *pKeyEvent = NULL;

/*static gboolean
cursor_changed_event(GtkWidget *tree_view, gpointer data )
{
	//if(pKeyEvent->keyval == GDK_KEY_Control_L)
	{
	  //gdk_event_put((GdkEvent*)pKeyEvent);
	  cout << "cur changed:" <<  endl;
	  //gdk_event_free ((GdkEvent*)pKeyEvent);
	}
}*/

/*static gboolean
mov_cursor_event(GtkWidget *tree_view, GtkMovementStep arg1, gint arg2, gpointer data )
{
	cout << "move cur:" << arg2 << endl;
}*/

/*static gboolean
row_activated_event(GtkWidget *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer data )
{
	cout << "row activated:" << endl;
	//gdk_event_free ((GdkEvent*)pKeyEvent);
	//g_signal_emitv(&gVal, kdb_handler_id/*row_activated_id /, (GQuark)0, NULL);
	//g_signal_emit_by_name(G_OBJECT (irbis_tree_view), "key-release-event");
}*/

/*static gboolean
kbd_release_event ( GtkWidget *text_view, GdkEventKey *event)
{
	/*switch(event->type)
       {
         case GDK_KEY_PRESS:  cout << "press" << endl; break;
         case GDK_KEY_RELEASE:cout << "release" << endl; break;
       } /
       //gdk_event_get();
       if(event->keyval == GDK_KEY_Control_L)
	   {
	     cout << "key released" <<  endl;
	     //g_signal_emit_by_name(G_OBJECT (change_mark), "button-release-event");
	     //gdk_event_free ((GdkEvent*)pKeyEvent);
	   }
}*/

static gboolean
kbd_press_event ( GtkWidget *widget, GdkEventKey *event)
{
       guint32 _time;
       guint *keyval;
       GdkKeymap keymap;
       GdkModifierType consumed;
       /*gdk_keymap_translate_keyboard_state (&keymap, event->hardware_keycode,
                                            (GdkModifierType)event->state, event->group, keyval,
                                            NULL, NULL, &consumed);*/
    //if(event->keyval == GDK_Control_L)
    if( (event->state & GDK_CONTROL_MASK ) == GDK_CONTROL_MASK )
    {
        if ( (event->keyval == GDK_KEY_R) || (event->keyval == GDK_KEY_r))
            g_signal_emit_by_name(G_OBJECT (mark_check), "clicked");
    }
    {
      if ( (/*(event->keyval == GDK_Up ||  event->keyval ==  GDK_KP_Up)*/ event->keyval ==  GDK_Clear || event->keyval ==  GDK_KP_Begin
       /*(event->keyval == GDK_Down ||  event->keyval == GDK_KP_Down)*/) &&
      !gtk_widget_is_focus (file_tree_view)  /*&& !bfocus_in_text*/ )
      gtk_widget_grab_focus (file_tree_view);

      if ( (event->keyval == GDK_grave) && !gtk_widget_is_focus (file_tree_view)  && !bfocus_in_text )
      {
         //g_signal_emit_by_name(G_OBJECT (auto_inc_mark_toggle), "toggled");
         gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (auto_inc_check), !bauto_inc);
         if(is_mark_changed)
                 Sync_Mark();
         //bauto_inc ? GetNextMark() : 0;
      }
      //cout << "Keycode:" << event->hardware_keycode << " " << gdk_keyval_name(event->keyval) <<  endl;

       if(event->keyval == GDK_Escape )
       {
            if(is_mark_changed) Sync_Mark();
            GetNextMark();//chMarkDir);
       }

       if ( (bfocus_in_text && is_marking) && (event->keyval == GDK_space) )
       {
         short count = psys->get_marks_num();
         //gint counter = 0;
         GtkTextIter iter;
         //gint buffer_x, buffer_y;

         /*gtk_text_view_window_to_buffer_coords(GTK_TEXT_VIEW(text_view),
                                             GTK_TEXT_WINDOW_WIDGET,
                                                       (gint)gText_view_x,
                                                       (gint)gText_view_x,
                                                       &buffer_x,
                                                       &buffer_y);*/

         gtk_text_view_get_iter_at_location  (GTK_TEXT_VIEW(text_view),
                                                         &iter,
                                                         gText_view_x, gText_view_y);

         /*if( !is_precise_hit  && g_mark_indx < count-1)
              gtk_text_iter_backward_find_char (&iter,  check_for_ws,  &counter, NULL);*/

            //gtk_text_iter_forward_chars(&iter, counter);

            gtk_text_buffer_place_cursor (text_buffer,  dynamic_cast<const GtkTextIter *>(&iter));

            gtk_text_buffer_insert_with_tags_by_name (text_buffer, &iter,
                                                         (const gchar*)m_mark, -1,
                                                         "blue_foreground", NULL);

           if( !GTK_WIDGET_IS_SENSITIVE(marking_done) )
                 gtk_widget_set_sensitive(marking_done, TRUE);

          if( bauto_inc && g_mark_indx < psys->get_marks_num()  )
          {
              //if(is_mark_changed) Sync_Mark();
                GetNextMark();
             gtk_entry_set_text(GTK_ENTRY(mark_view), (const gchar *)m_mark);
          }

		  //GdkEventButton *mouse_btn_event = new GdkEventButton;
		  //mouse_btn_event->button = 1;
          //g_signal_emit_by_name(G_OBJECT (text_view), "button-press-event", mouse_btn_event);
          //delete mouse_btn_event;
       }

       if((/*!bfocus_in_text &&*/ is_marking) && (/*event->keyval == GDK_space ||*/
                                                               event->keyval == GDK_1 ||
                                                               event->keyval == GDK_2))//Control_L)
       //if( event->hardware_keycode == 37 )
       {
          //pKeyEvent = (GdkEventKey*)gdk_event_copy((GdkEvent*)event);

         /*GtkTreeIter iter, parent_iter;
         GtkTreePath *path;
         GtkTreeViewColumn *column;*/

         /*switch(gdk_keyboard_grab(event->window, FALSE, event->time))
         {
             case GDK_GRAB_SUCCESS: cout << "allright" << endl; break;
             case GDK_GRAB_ALREADY_GRABBED: cout << "ALREADY_GRABBED" << endl; break;
             case GDK_GRAB_INVALID_TIME: cout << "INVALID_TIME" << endl; break;
             case GDK_GRAB_NOT_VIEWABLE: cout << "NOT_VIEWABLE" << endl; break;
             case GDK_GRAB_FROZEN: cout << "FROZEN" << endl; break;
         }*/


         //gtk_widget_grab_focus (GTK_WIDGET(irbis_tree_view));

         //chMarkDir = 0;
         (event->keyval == GDK_1) ? gMarkDir = 0 : event->keyval == GDK_2 ? gMarkDir = 1 : gMarkDir;
         if(is_mark_changed) Sync_Mark();
         GetNextMark();//chMarkDir);

         /*gtk_tree_view_get_cursor(GTK_TREE_VIEW(irbis_tree_view), &path, NULL);
         gtk_tree_path_next(path);
         gtk_tree_selection_select_path (selection, path);
         gtk_tree_view_set_cursor(GTK_TREE_VIEW(irbis_tree_view), path, NULL, FALSE);
         gtk_tree_path_free (path);*/

       //g_signal_emit_by_name(G_OBJECT (change_mark), "clicked");//good but not enough
       //g_signal_emit_by_name(G_OBJECT (irbis_tree_view), "key-press-event"); //failed

       //return FALSE;
       //cout << "Keycode:" << event->hardware_keycode << (event->state & GDK_CONTROL_MASK) << endl;

       //g_signal_emitv(&gVal, kdb_handler_id, (GQuark)0, NULL);

       }
       //cout << "Keycode:" << event->hardware_keycode << " " << (event->state & GDK_CONTROL_MASK) << " " << event->keyval << endl;
       /*switch(event->type)
       {
         case GDK_KEY_PRESS:  cout << "press" << endl; break;
         case GDK_KEY_RELEASE:cout << "release" << endl; break;
       }*/
    }
    //gdk_keyboard_ungrab(event->time);
        switch (event->keyval)
        {
           case GDK_F1:
           {
                //help_index = 1;
                g_signal_emit_by_name (GTK_OBJECT (aboutus), "clicked");
                //help_index = 0;
           }break;
           case GDK_F2:
           {
               //cout << "F2" << endl;
               if( GTK_WIDGET_IS_SENSITIVE(save_all_cards) )
               {
                  g_signal_emit_by_name(G_OBJECT (save_all_cards), "clicked");
               }
           }break;
           case GDK_F3:
            {
               g_signal_emit_by_name(G_OBJECT (load_cards), "clicked");
            }break;
           case GDK_F5:
            {
                if( GTK_WIDGET_IS_SENSITIVE(auto_put_tags))
                    g_signal_emit_by_name(G_OBJECT (auto_put_tags), "clicked");
            }break;
            //case GDK_Alt_R:
            case GDK_Control_L:
            {
                gMarkDir = 0; /*g_mark_indx = psys->get_marks_num()-1;*/
                g_signal_emit_by_name(G_OBJECT (change_mark), "clicked");
                //g_signal_emit_by_name(G_OBJECT (change_mark), "button-press-event");
            }break;
            //case GDK_Control_R:
            case GDK_Alt_L:
            {
                    gMarkDir = 1; /*g_mark_indx = 0;*/
                    g_signal_emit_by_name(G_OBJECT (change_mark), "clicked");

            }break;
           case GDK_F7:
           {
               g_signal_emit_by_name(GTK_OBJECT (font_btn), "clicked");
           } break;
           case GDK_F9:
           {
               //cout << "F9" << endl;
             if( GTK_WIDGET_IS_SENSITIVE(marking_done) )
             {
                g_signal_emit_by_name (G_OBJECT (marking_done), "clicked");
             }
           }break;
           case GDK_F10:
           {
             //quit the game
             g_signal_emit_by_name (G_OBJECT (window), "destroy");
           }break;
           default:break;
        }
    /*else*/ return false;
}

void about_us( GtkWidget *widget,
            gpointer   data )
{

   gchar *authors[] = { " based on code by TKL K96 207", NULL };


   gtk_show_about_dialog (GTK_WINDOW(data),
                          "program-name", prog_name,
                          "copyright", "Muqobil Dasturlar To'plami (c) 2009-2019\nEvaluation edition",
                          "license", "The source code is released under the terms of GPL version 3.",
                          "version", ver,
                          "comments", "O'zbekiston, Toshkent shahri\n  muqobildasturlar@gmail.com",
                          "title", ("About GTasnif"),
                          "authors", authors,
                         NULL);

}

/*void about_us( GtkWidget *widget,
            gpointer   data )
{
   GtkWidget *dialog;

   dialog = gtk_message_dialog_new ((GtkWindow*)data,
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_INFO,
                                  GTK_BUTTONS_OK,
                                  " GTasnif ver 1.15\n"
				  " Muqobil Dasturlar To`plami (c) 2009-2017\n"
                                  " TKL K96 207\n"
                                  " Released under the terms of GPL\n"
				  " O`zbekiston, Toshkent 1427-1431.\n  muqobildasturlar@gmail.com \n"
				  "Developed on FreeBSD using Wine\n"
				  "with MinGW via Code::Blocks" );
   /*dialog = gtk_message_dialog_new ((GtkWindow*)data,
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_INFO,
                                  GTK_BUTTONS_OK, help_wanted[help_index]);
   gtk_dialog_run (GTK_DIALOG (dialog));/
   gtk_widget_destroy (dialog);
}*/

static gboolean  gdk_event_conf  (GtkWidget          *widget,
                                  GdkEventConfigure  *event,
                                  gpointer            user_data)
{
    gl_width = event->width ;
    gl_height = event->height;

    return FALSE;
}

void  gtk_main_loop_quit(void)
{
    gl_vpaned_pos = gtk_paned_get_position( GTK_PANED (vpaned) );
    gl_hpaned_pos = gtk_paned_get_position( GTK_PANED (hpaned) );

    //cout << gl_width << " -- " << gl_height<< endl;

    GlobalDone();

    gtk_main_quit();
    gtk_widget_destroy(GTK_WIDGET(window));
}

int main( int   argc,
          char *argv[] )
{
 GtkWidget *button;
 GtkWidget *sw2, *scrolled_window;
 GtkWidget *frame1,  *ctrl_frame, *hbox;
 GtkWidget *main_vbox, *main_hbox;
 GtkWidget  *sw;
 //GtkWidget *auto_inc_check, *precise_hit;// , *hpaned;

 strncpy(buf, prog_name, sizeof(buf));
 strncat(buf, "-", sizeof(buf));
 strncat(buf, ver, sizeof(buf));
 app_title.assign(buf);

 //psys = new CSystem();
 psys = new CTasnif();
 foo = new Irbis_Item();
 flist = new File_Item();

 gtk_init (&argc, &argv);

 window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
 //gchar *app_name_ver = g_strjoin(" ", prog_name, ver);


 gtk_window_set_title(GTK_WINDOW(window), buf);

 //g_free(app_name_ver);
 //gtk_widget_set_size_request (GTK_WIDGET (window), 790, 550);

 g_signal_connect (G_OBJECT (window), "destroy",
                       G_CALLBACK (gtk_main_loop_quit), NULL);
 g_signal_connect_swapped (G_OBJECT (window), "delete_event",
                               G_CALLBACK (gtk_widget_destroy),
                               window);

 g_signal_connect_swapped (G_OBJECT (window), "configure_event",
                               G_CALLBACK (gdk_event_conf),
                               window);
 kdb_handler_id = g_signal_connect/*object*/ (G_OBJECT (window),
                             "key-press-event",
                             G_CALLBACK (kbd_press_event),
                             NULL);
                            // (GConnectFlags)0) ;

 sw = gtk_scrolled_window_new (NULL, NULL);
      gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
                                           GTK_SHADOW_ETCHED_IN);
      gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
                                      GTK_POLICY_AUTOMATIC,
                                      GTK_POLICY_AUTOMATIC);

 //gtk_widget_set_size_request (GTK_WIDGET (sw), 370, 250);

 text_view = gtk_text_view_new ();
 gtk_text_view_set_justification(GTK_TEXT_VIEW (text_view ),  GTK_JUSTIFY_CENTER);
 gtk_text_view_set_editable( GTK_TEXT_VIEW (text_view ), true);
 gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW (text_view ), GTK_WRAP_WORD_CHAR);


 //connect buffer with view
 text_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));

 gtk_text_buffer_create_tag (text_buffer, "blue_foreground",
			      "foreground", "blue", NULL);

 //gtk_text_buffer_set_text (text_buffer, " ", -1);

   scrolled_window = gtk_scrolled_window_new (NULL, NULL);
   gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
		   	           GTK_POLICY_AUTOMATIC,
				   GTK_POLICY_AUTOMATIC);

     GtkWidget *box;
    box = gtk_event_box_new ();
    gtk_container_add (GTK_CONTAINER (box), text_view);
    gtk_widget_add_events (box,
                 GDK_POINTER_MOTION_HINT_MASK |
                 GDK_BUTTON_PRESS_MASK |
                 GDK_BUTTON_RELEASE_MASK /*|
                 GDK_SMOOTH_SCROLL_MASK |
                 GDK_TOUCH_MASK*/);
    g_signal_connect (box, "event",
                        G_CALLBACK (enter_notify_event), NULL);

     gtk_container_add (GTK_CONTAINER (scrolled_window), box);//text_view);

     g_signal_connect (G_OBJECT (text_view), "button-press-event",
                     G_CALLBACK (button_press_event), NULL);
     /*g_signal_connect (G_OBJECT (text_view), "enter-notify-event",
                     G_CALLBACK (enter_notify_event), NULL);*/

     g_signal_connect (G_OBJECT (text_view), "focus-in-event",
                     G_CALLBACK (focus_in_event), NULL);
     g_signal_connect (G_OBJECT (text_view), "focus-out-event",
                     G_CALLBACK (focus_out_event), NULL);

     main_hbox = gtk_hbox_new (FALSE, 0);
     /* Put the box into the main window. */
     // gtk_container_add (GTK_CONTAINER (window), hbox);

     main_vbox = gtk_vbox_new (FALSE, 0);
     /* Put the box into the main window. */
     gtk_container_add (GTK_CONTAINER (main_vbox), main_hbox);
     gtk_container_add (GTK_CONTAINER (window), main_vbox);



     //list of cards
     gchar *titles[2] = { "Cards list", "IRBIS fields" };
     //irbis_field_list = gtk_clist_new_with_titles( 1, &titles[1]);
     //gtk_box_pack_start (GTK_BOX (main_hbox),irbis_field_list,TRUE, TRUE, 10);

     hpaned = gtk_hpaned_new ();
     //gtk_widget_set_size_request (hpaned, 370, -1);

     /* create models */
      irbis_items_model = create_items_model ();
      files_list_model  = create_files_list_model();
      //cout << "Step 1" << endl;

    /* create tree view */
    irbis_tree_view = gtk_tree_view_new_with_model (irbis_items_model);
    gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (irbis_tree_view), TRUE);
    gtk_tree_selection_set_mode (gtk_tree_view_get_selection (GTK_TREE_VIEW (irbis_tree_view)),
                                   GTK_SELECTION_SINGLE);
     gtk_tree_view_set_enable_search(GTK_TREE_VIEW (irbis_tree_view), false);
     //g_value_init_from_instance(&gVal, irbis_tree_view);
     //g_assert(G_VALUE_HOLDS_OBJECT(&gVal));
     /*treeview_kdb_handler_id = g_signal_connect/*object / (G_OBJECT (irbis_tree_view),
                             "key-press-event",
                             G_CALLBACK (treebiew_kbd_press_event),
                             NULL);*/
     /*g_signal_connect/*object/ (G_OBJECT (irbis_tree_view),
                             "key-release-event",
                             G_CALLBACK (kbd_release_event),
                             NULL);*/
     /*row_activated_id = g_signal_connect (G_OBJECT (irbis_tree_view),
                             "row-activated",//"move-cursor",
                             G_CALLBACK (row_activated_event),//(mov_cursor_event),
                             NULL);*/
     /*g_signal_connect/*object/ (G_OBJECT (irbis_tree_view),
                             "cursor-changed",
                             G_CALLBACK (cursor_changed_event),
                             NULL);*/
     //pKeyEvent = (GdkEventKey*)gdk_event_new(GDK_KEY_PRESS);

    //file list tree model
    file_tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL(files_list_model));
    gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (file_tree_view), TRUE);
    gtk_tree_selection_set_mode (gtk_tree_view_get_selection (GTK_TREE_VIEW (file_tree_view)),
                                   GTK_SELECTION_SINGLE);
    gtk_tree_view_set_enable_search(GTK_TREE_VIEW (file_tree_view ), false);

    add_columns (GTK_TREE_VIEW (irbis_tree_view), irbis_items_model);
    g_object_unref (irbis_items_model);

    add_columns_files (GTK_TREE_VIEW (file_tree_view), GTK_TREE_MODEL(files_list_model));
    g_object_unref (files_list_model);

    /* Setup the selection handler */
    //GtkTreeSelection *select, *sel;

    selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (irbis_tree_view));
    gtk_tree_selection_set_mode (selection, GTK_SELECTION_SINGLE);
    g_signal_connect (G_OBJECT (selection), "changed",
                  G_CALLBACK (tree_selection_changed_cb),
                  NULL);

    sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (file_tree_view));
    gtk_tree_selection_set_mode (sel, GTK_SELECTION_SINGLE);
    g_signal_connect (G_OBJECT (sel), "changed",
                  G_CALLBACK (files_selection_changed_cb),
                  NULL);

    gtk_container_add (GTK_CONTAINER (sw), irbis_tree_view);
    //gtk_box_pack_start (GTK_BOX (main_hbox), sw, TRUE, TRUE, 0);
    //gtk_box_pack_start (GTK_BOX (main_hbox),irbis_tree_view,TRUE, TRUE, 10);


     //Group box
     frame1 = gtk_frame_new ("Matn");
     //gtk_widget_set_size_request (GTK_WIDGET (frame1), 250, 350);
     //gtk_widget_set_size_request (GTK_WIDGET (frame1), 150, -1);

     gtk_container_add (GTK_CONTAINER (frame1), scrolled_window);


     /* create a vpaned widget and add it to our toplevel window */
     vpaned = gtk_vpaned_new ();
     gtk_paned_add2(GTK_PANED(hpaned), vpaned);
     gtk_paned_add1(GTK_PANED(hpaned), sw);

     //gtk_widget_set_size_request (vpaned, 200, -1);

     //gtk_container_add (GTK_CONTAINER (vpaned), frame1);
     //gtk_paned_pack1 (GTK_PANED(vpaned), frame1, FALSE, TRUE);
     gtk_paned_add1(GTK_PANED(vpaned), frame1);
     //gtk_box_pack_start (GTK_BOX (main_hbox),vpaned,TRUE, TRUE, 5);
     //gtk_paned_pack1 (GTK_PANED(main_hbox), vpaned, FALSE, TRUE);
     //gtk_widget_show (vpaned);


      sw2 = gtk_scrolled_window_new (NULL, NULL);
      gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw2),
                                           GTK_SHADOW_ETCHED_IN);
      gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw2),
                                      GTK_POLICY_AUTOMATIC,
                                      GTK_POLICY_AUTOMATIC);

     //cards_list = gtk_clist_new_with_titles( 1, titles);
     //gtk_box_pack_start (GTK_BOX (main_hbox),cards_list,TRUE, TRUE, 0);
     //gtk_container_add (GTK_CONTAINER (sw2), cards_list);
     gtk_container_add (GTK_CONTAINER (sw2), file_tree_view);

     gtk_paned_add2 (GTK_PANED(vpaned), sw2);
     //gtk_box_pack_start (GTK_BOX (main_hbox),vpaned,TRUE, TRUE, 5);
     gtk_box_pack_start (GTK_BOX (main_hbox),hpaned,TRUE, TRUE, 5);

     /* When a selection is made, we want to know about it. The callback
     * used is selection_made */
     /*gtk_signal_connect(GTK_OBJECT(cards_list), "select_row",
                       GTK_SIGNAL_FUNC(selection_made), NULL);*/

     ctrl_frame = gtk_frame_new ("Controls");
     gtk_box_pack_start (GTK_BOX (main_vbox),ctrl_frame,FALSE, FALSE, 10);
     //gtk_widget_set_size_request (GTK_WIDGET (ctrl_frame), 200, 50);

  /*--------------------------------mark hit place-----------------*/
   /*frame2 = gtk_frame_new ("test");
   gtk_box_pack_start (GTK_BOX (main_vbox), frame, TRUE, TRUE, 0);*/

   hbox = gtk_hbox_new (FALSE, 0);
   gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
   gtk_container_add (GTK_CONTAINER (ctrl_frame), hbox);

   //mark_view = gtk_text_view_new ();

   //mark_vw_buffer =  gtk_text_buffer_new( NULL );
   mark_view = gtk_entry_new();//gtk_text_view_new_with_buffer ( mark_vw_buffer );
   gtk_entry_set_editable (GTK_ENTRY(mark_view), FALSE);

   gtk_widget_set_size_request (GTK_WIDGET (mark_view), 35, 25);

   //gtk_text_view_set_editable( GTK_TEXT_VIEW (mark_view ), true);
   //gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW (mark_view ), GTK_WRAP_WORD);

   //connect buffer with view
   //mark_vw_buffer =  gtk_text_view_get_buffer (GTK_TEXT_VIEW (mark_view));
   //gtk_text_buffer_set_text (mark_vw_buffer, "TEST ", -1);

   //Mnemonics for acceleration of access
   change_mark = gtk_button_new_with_mnemonic("Ctrl/Alt белги ўзгаруви");//("_Change");
   g_signal_connect (G_OBJECT (change_mark), "clicked",
            G_CALLBACK (mark_change_event), NULL);//&gMarkDir);
   /*g_signal_connect (G_OBJECT (change_mark), "button-press-event",
            G_CALLBACK (mark_pressed_event), NULL);//&gMarkDir);*/
   /*g_signal_connect (G_OBJECT (change_mark), "button-release-event",
            G_CALLBACK (mark_release_event), NULL);//&gMarkDir);*/

   gtk_box_pack_start (GTK_BOX (hbox),change_mark,FALSE, FALSE, 10);

   //bind mnemonic with hit place
   //gtk_label_set_mnemonic_widget (GTK_LABEL(change_mark),  mark_view);

   //gtk_container_add (GTK_CONTAINER (ctrl_frame), mark_view);
   gtk_box_pack_start (GTK_BOX (hbox),mark_view,FALSE, FALSE, 10);

   g_signal_connect (G_OBJECT (mark_view), "button-press-event",
		    G_CALLBACK (mark_hit_event), NULL);

   //gtk_widget_show (mark_view);
  /*--------------------------------mark hit place-----------------*/

/*-----------------------Load cards --------------------------------- */
   //Mnemonics for acceleration of access
   load_cards = gtk_button_new_with_mnemonic("Юкла F3");//("_Load cards");
   g_signal_connect (G_OBJECT (load_cards), "clicked",
		    G_CALLBACK (load_cards_event), window);

   gtk_box_pack_start (GTK_BOX (hbox),load_cards,FALSE, FALSE, 10);
/*---------------------------------------------------------------------*/


/*-----------------------Done with marking --------------------------------- */
   //Mnemonics for acceleration of access
   marking_done = gtk_button_new_with_mnemonic("Сарала F9");//("_Done");
   g_signal_connect (G_OBJECT (marking_done), "clicked",
		    G_CALLBACK (mark_done_event), window);

   gtk_widget_set_sensitive(marking_done, FALSE);
   gtk_box_pack_start (GTK_BOX (hbox),marking_done,FALSE, FALSE, 10);
/*---------------------------------------------------------------------*/

/*-----------------------Save All cards into file ------------------------------ */
   //Mnemonics for acceleration of access
   save_all_cards = gtk_button_new_with_mnemonic("Ўгир F2");//("_Save & Convert ");
   g_signal_connect (G_OBJECT (save_all_cards), "clicked",
		    G_CALLBACK (save_all_cards_event), window);

   gtk_box_pack_start (GTK_BOX (hbox),save_all_cards,FALSE, FALSE, 10);
/*---------------------------------------------------------------------*/


/*-------------------- Auto put tags onto cards ---------------------- */
   //Mnemonics for acceleration of access
   auto_put_tags = gtk_button_new_with_mnemonic("Тэгларни Кўй F5");//("_Save & Convert ");
   g_signal_connect (G_OBJECT (auto_put_tags), "clicked",
		    G_CALLBACK (auto_put_tags_event), window);

   gtk_widget_set_sensitive(auto_put_tags, FALSE);
   gtk_box_pack_start (GTK_BOX (hbox), auto_put_tags, FALSE, FALSE, 10);
/*---------------------------------------------------------------------*/


/*---------------------------------font selection-----------------------*/
 /*font_btn = gtk_font_button_new ();
 gtk_box_pack_start (GTK_BOX (hbox), font_btn, TRUE, TRUE, 0);
 g_signal_connect (GTK_OBJECT (font_btn), "font-set",
 			GTK_SIGNAL_FUNC (font_select), window);
 gtk_widget_show (font_btn);*/
/*----------------------------end of font selection---------------------*/

/*------------------------------------------------prefrences-------------------------------------*/
button = gtk_button_new_from_stock(GTK_STOCK_PREFERENCES);
     g_signal_connect (button, "clicked",
			G_CALLBACK (conf_dialog_clicked), NULL);
     gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 10);
/*------------------------------end of prefrences---------------------------------------------*/


/*--------------------------mark check--------------------------*/
  //mark_check = gtk_check_button_new_with_label ("_Markable");
   mark_check = gtk_check_button_new_with_mnemonic("Белгилаш");//("_Markable");
  gtk_box_pack_start (GTK_BOX (hbox), mark_check, TRUE, TRUE, 0);
  //gtk_container_add (GTK_CONTAINER (ctrl_frame), mark_check);
  g_signal_connect (G_OBJECT (mark_check), "toggled",
               G_CALLBACK (mark_toggle), NULL);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (mark_check), TRUE);
  gtk_widget_show (mark_check);
/*--------------------------mark check--------------------------*/

/*--------------------------auto increment mark check--------------------------*/
  auto_inc_check = gtk_check_button_new_with_mnemonic("Ўзгартириб бор");//("_Auto Increment");
  gtk_box_pack_start (GTK_BOX (hbox), auto_inc_check, TRUE, TRUE, 0);
  //gtk_container_add (GTK_CONTAINER (ctrl_frame), mark_check);
  g_signal_connect (G_OBJECT (auto_inc_check), "toggled",
               G_CALLBACK (auto_inc_mark_toggle), NULL);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (auto_inc_check), FALSE);
  gtk_widget_show (auto_inc_check);
/*--------------------------mark check--------------------------*/

/*----------------------precise hit toggle----------------------------*/
  precise_hit = gtk_check_button_new_with_mnemonic("Аник");//("_Markable");
  gtk_box_pack_start (GTK_BOX (hbox), precise_hit, TRUE, TRUE, 0);
  //gtk_container_add (GTK_CONTAINER (ctrl_frame), mark_check);
  g_signal_connect (G_OBJECT (precise_hit), "toggled",
               G_CALLBACK (precise_hit_toggle), NULL);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (precise_hit), is_precise_hit);
  gtk_widget_show (precise_hit);

/*----------------------precise hit toggle----------------------------*/


 //about us if anybody wish to know
 aboutus = gtk_button_new_from_stock(GTK_STOCK_ABOUT);
 g_signal_connect (G_OBJECT (aboutus), "clicked",
 			G_CALLBACK (about_us), window);
 gtk_box_pack_start (GTK_BOX (hbox),aboutus,FALSE, FALSE, 10);


/*--------------------------------keyboard snooper--------------------------- */
   //guint snoop_id;
   //snoop_id = gtk_key_snooper_install ( key_snoop_cb, NULL);
/*--------------------------------keyboard snooper--------------------------- */

 files.reserve(1);
 /*files.push_back("1.TXT");
 files.push_back("2.TXT");
 files.push_back("3.TXT");
 files.push_back("4.TXT");*/

 //psys->set_cards_files(files);
 //cout << psys->load_next_card(0) << endl;
 //printf("%s", psys->load_next_card(3));

  //gtk_widget_show_all (window);

  GlobalInit();

  gtk_paned_set_position( GTK_PANED (vpaned), gl_vpaned_pos );
  gtk_widget_show (vpaned);

  gtk_paned_set_position( GTK_PANED (hpaned), gl_hpaned_pos );
  gtk_widget_show (hpaned);

  gtk_widget_set_size_request (GTK_WIDGET (window), gl_width, gl_height);
  gtk_window_maximize(GTK_WINDOW(window));

  gtk_widget_show_all (window);
  //cout << "Step 2" << endl;

  gtk_main ();

  //gtk_key_snooper_remove( snoop_id );
 //GlobalDone();

 delete psys;
 delete foo;//Irbis_Item foo
 delete flist;

 return 0;
}

