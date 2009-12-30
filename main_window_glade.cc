/*
  	Filename: .cc
	Author: Kreil Dominik	
	Description:

	Aoide is a simple musicplayer using the file structure
*/

// DO NOT EDIT THIS FILE ! It was created using glade-- 
// for gtk 2.8.3 and gtkmm 2.8.0
//
// Please modify the corresponding derived classes in ./src/main_window.cc


#if defined __GNUC__ && __GNUC__ < 3
#error This program will crash if compiled with g++ 2.x
// see the dynamic_cast bug in the gtkmm FAQ
#endif //
#include "config.h"
#include <gtkmmconfig.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#include <sigc++/compatibility.h>
#define GMM_GTKMM_22_24(a,b) b
#else //gtkmm 2.2
#define GMM_GTKMM_22_24(a,b) a
#endif //
#include "main_window_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>


main_window_glade::main_window_glade() : Gtk::Window(Gtk::WINDOW_TOPLEVEL), treeview(new string("/")), mainTable(10,20)
{  
	main_window = this;
	gmm_data = new GlademmData(get_accel_group());
	main_window->set_title("aoide Musikplayer");
	main_window->set_modal(false);
	main_window->property_window_position().set_value(Gtk::WIN_POS_CENTER);
	main_window->set_resizable(true);
	main_window->property_destroy_with_parent().set_value(false);
	cerr << "seting up layout" << endl;
	setUpLayout();

	//some init stuff
	player.setActionHandler(this);
	player.setVol(0.5);
	maximize();
	actualTrack = "";
	playlistPath="playlist.lst";
	loadPlaylist(playlistPath);
	actualTrackNumber = 0;
	loopOverList=false;
	loopOverTrack=false;	

	cerr << "inite step" << endl;
	
	main_window->show();
	main_window->show_all_children();
	main_window->signal_delete_event().connect(SigC::slot(*this, &main_window_glade::quit), false);
	cerr << "init done" << endl;
}

void main_window_glade::setUpLayout()
{
	static Layout layout;
	verticalLeftScroll.add(treeview);
	verticalLeftScroll.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	verticalLeft.pack_start(verticalLeftScroll);
	//layout.add(verticalLeft);

	verticalRightScroll.add(playlist);
	verticalRightScroll.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC); 
	verticalRight.pack_start(verticalRightScroll);

	int size = 70;
	imPause = new Image((Image("images/pause.png").get_pixbuf()->scale_simple(size,size,Gdk::INTERP_BILINEAR)));
	imPlay = new Image((Image("images/play.png").get_pixbuf()->scale_simple(size,size,Gdk::INTERP_BILINEAR)));
	imNext = new Image((Image("images/next.png").get_pixbuf()->scale_simple(size,size,Gdk::INTERP_BILINEAR)));
	imPrev = new Image((Image("images/prev.png").get_pixbuf()->scale_simple(size,size,Gdk::INTERP_BILINEAR)));
	imStop = new Image((Image("images/stop.png").get_pixbuf()->scale_simple(size,size,Gdk::INTERP_BILINEAR)));
	imRepTrack = new Image((Image("images/rep_track.png").get_pixbuf()->scale_simple(size,size,Gdk::INTERP_BILINEAR)));
	imRepAll = new Image((Image("images/rep_list.png").get_pixbuf()->scale_simple(size,size,Gdk::INTERP_BILINEAR)));
	imDelTrack = new Image((Image("images/del_track.png").get_pixbuf()->scale_simple(size,size,Gdk::INTERP_BILINEAR)));
	imDelAll = new Image((Image("images/del_all.png").get_pixbuf()->scale_simple(size,size,Gdk::INTERP_BILINEAR)));
	imAbout = new Image((Image("images/about.png").get_pixbuf()->scale_simple(size,size,Gdk::INTERP_BILINEAR)));
	
	setButtonToPlay(); 
	bStop.add(*imStop);
	nextTrack.add(*imNext);
	prevTrack.add(*imPrev);
	deleteTrack.add(*imDelTrack);
	deleteAll.add(*imDelAll);
	trackLoop.add(*imRepTrack);
	listLoop.add(*imRepAll);
	about.add(*imAbout);
	
	listLoop.signal_toggled().connect_notify(sigc::mem_fun(*this, &main_window_glade::onToggleListLoop));
	trackLoop.signal_toggled().connect_notify(sigc::mem_fun(*this, &main_window_glade::onToggleTrackLoop));
	bPauseResume.signal_clicked().connect_notify(sigc::mem_fun(*this, &main_window_glade::onPlay));
	bStop.signal_clicked().connect_notify(sigc::mem_fun(*this, &main_window_glade::onStop));
	nextTrack.signal_clicked().connect_notify(sigc::mem_fun(*this, &main_window_glade::onNextTrack));
	prevTrack.signal_clicked().connect_notify(sigc::mem_fun(*this, &main_window_glade::onPrevTrack));
	deleteTrack.signal_clicked().connect_notify(sigc::mem_fun(*this, &main_window_glade::onDeleteTrack));
	deleteAll.signal_clicked().connect_notify(sigc::mem_fun(*this, &main_window_glade::onDeleteAll));
	about.signal_clicked().connect_notify(sigc::mem_fun(*this, &main_window_glade::onAbout));
	
	vol.set_range(0.0, 1.0);
	vol.set_increments(0.01, 0.01);
	vol.set_value(0.5);
	vol.signal_change_value().connect_notify(sigc::mem_fun(*this, &main_window_glade::onSetVol));
	time.set_range(0.0, 1.0);
	time.set_increments(0.01, 0.01);
	time.signal_change_value().connect_notify(sigc::mem_fun(*this, &main_window_glade::onSeek));

	bPauseResume.set_tooltip_text("Play/Pause/Resume");
	prevTrack.set_tooltip_text("previous Track");
	nextTrack.set_tooltip_text("next Track");	
	bStop.set_tooltip_text("Stop playing");
	deleteTrack.set_tooltip_text("delte selected Track from Playlist");
	deleteAll.set_tooltip_text("delete the whole Playlist");
	trackLoop.set_tooltip_text("repeat actual Track");
	listLoop.set_tooltip_text("repeat throught Playlist");
	about.set_tooltip_text("about");

	playerButtons.add(bPauseResume);
	playerButtons.add(prevTrack);
	playerButtons.add(nextTrack);
	playerButtons.add(bStop);
	playerButtons.add(deleteTrack);
	playerButtons.add(deleteAll);
	playerButtons.add(trackLoop);
	playerButtons.add(listLoop);
	playerButtons.add(about);	

	playerControl.add(time);
	playerControl.add(vol);
	
	mainPlayer.add(playerButtons);
	mainPlayer.add(playerControl);

	mainTable.set_border_width(10);
	verticalLeft.set_border_width(5);
	verticalRight.set_border_width(5);
	mainTable.attach(mainPlayer, 0,5,0,1);
	mainTable.attach(verticalLeft, 0,2,1,21);
	mainTable.attach(verticalRight, 2,11,1,21);
	
	mainTable.show_all_children();
	
	main_window->add(mainTable);
	
	playlist.add_events(Gdk::BUTTON_PRESS_MASK);
    playlist.signal_button_press_event().connect(sigc::mem_fun(*this, &main_window_glade::onPlaylistDoubleClick),false); 

	treeview.add_events(Gdk::BUTTON_PRESS_MASK);
    treeview.signal_button_press_event().connect(sigc::mem_fun(*this, &main_window_glade::onTreeDoubleClick),false); 
}

void main_window_glade::onPlay()
{
	if(player.isFinished())
	{
		player.play(actualTrack);
		set_title("aoide Musikplayer : "+actualTrack);
		time.set_range(0,player.getPlayLength()/1000.0);
		setButtonToPause();
		return;
	}				
	if(player.isPaused())
	{
		player.resume();
		setButtonToPause();
	}
	else
	{
		player.pause();
		setButtonToPlay();
	}
}


void main_window_glade::onNextTrack()
{
	if(playlist.getNumberOfTracks()==0) return;
	actualTrackNumber = (actualTrackNumber+1)%playlist.getNumberOfTracks();
	actualTrack = playlist.getRowPath(actualTrackNumber);
	cerr << "onNextTrack " << actualTrackNumber << " "<< actualTrack << endl;
	if (actualTrack != "")
	{
		player.play(actualTrack);
		set_title("aoide Musikplayer : "+actualTrack);
		time.set_range(0,player.getPlayLength()/1000.0);
	}
}


void main_window_glade::onPrevTrack()
{	
	if(playlist.getNumberOfTracks()==0) return;
	actualTrackNumber = (actualTrackNumber-1+playlist.getNumberOfTracks())%playlist.getNumberOfTracks();
	actualTrack = playlist.getRowPath(actualTrackNumber);
	cerr << "onPrevTrack " << actualTrackNumber << " "<< actualTrack << endl;
	if (actualTrack != "")
	{
		player.play(actualTrack);
		set_title("aoide Musikplayer : "+actualTrack);
		time.set_range(0,player.getPlayLength()/1000.0);
	}
}


void main_window_glade::onStop()
{
	player.stop();
	set_title("aoide Musikplayer");
	setButtonToPlay();
}

void main_window_glade::onSeek(const ScrollType &type, const double &val)
{
	player.seek(val*1000);
	cerr << "onSeek: " << endl;
}
	
void main_window_glade::onSetVol(const ScrollType &type, const double &val)
{
	player.setVol(val);
}

void main_window_glade::onDeleteTrack()
{
	RefPtr<Gtk::TreeView::Selection> selection = playlist.get_selection();

	if(selection->count_selected_rows() == 0) return;

	TreeModel::Row row = *(selection->get_selected());
	
	if(actualTrackNumber >= *(*selection->get_selected_rows().begin()).get_indices().begin())
		actualTrackNumber--;

	playlist.deleteTrack(selection->get_selected());
}

void main_window_glade::onDeleteAll()
{
	playlist.deleteAll();
	cerr << "onDeleteAll " << endl;
}


void main_window_glade::onToggleTrackLoop()
{
	loopOverTrack = !loopOverTrack;
	cerr << "onToggleTracLoop: " << loopOverTrack << endl;
}
void main_window_glade::onToggleListLoop()
{
	loopOverList = !loopOverList;
	cerr << "onTogglListLoop: " << loopOverList << endl;
}

void main_window_glade::onTrackFinished()
{
	cerr << "onTrackFinished " << playlist.getNumberOfTracks()  << "\t" << actualTrackNumber << endl;
	if(playlist.getNumberOfTracks() == actualTrackNumber+1)
	{	if(!loopOverList)
			return;			//finished
	}
	if(!loopOverTrack)
		actualTrackNumber = (actualTrackNumber+1)%playlist.getNumberOfTracks();
	actualTrack = playlist.getRowPath(actualTrackNumber);
	set_title("aoide Musikplayer");
	cerr << "onTrackFinished " << actualTrackNumber << " "<< actualTrack << endl;
	if (actualTrack != "")
	{
		player.play(actualTrack);
		set_title("aoide Musikplayer : "+actualTrack);
	}
}


void main_window_glade::onTrackPlays()
{
	cerr << "main_window_glade::onTrackPlays  "<< player.getTime() << endl;
	time.set_value(player.getTime()/1000);
	time.queue_draw();
	
}


void main_window_glade::appendDirToPlaylist(string path)
{
	DIR *hdir;
	struct dirent *entry;
	hdir = opendir(path.c_str());
	if (hdir == NULL)
	{
		cout << "error while opening " << path << endl;
	}
	do
	{
		entry = readdir(hdir);
		if (entry)
		{
			struct stat entryStatus;
			
			if (entry->d_name[0] == '.' || !strcmp(entry->d_name, ".."))	
				continue;
			string d_name = entry->d_name;
			string newPath = path + d_name;
			stat(newPath.c_str(), &entryStatus);
			int entryType = entryStatus.st_mode & S_IFMT;
			switch (entryType) {
				case S_IFREG: 	//file
								if(treeview.validateFileType(d_name))
									playlist.appendTitle(d_name, newPath);
								break;
				case S_IFLNK: //puts("Symbolischer Link"); 
								break;
				case S_IFDIR: //cout << "Verzeichnis: " << newPath << endl;
								break;
				default: 		//puts("Sonstiges");
								break;
			}
			//cout << entry->d_name << endl;
			//delete[] newPath;
		}
	} while (entry);
	closedir(hdir);	
}


bool main_window_glade::onPlaylistDoubleClick(GdkEventButton *event)
{
        static long unsigned lastclick = 0;

        cerr << "button press event detected" << endl;  

        if (event->type == GDK_2BUTTON_PRESS && event->button ==1)
        {
                if (lastclick == 0 || (event->time - lastclick) < 600)
                        lastclick = event->time;
                else
				{	
					RefPtr<Gtk::TreeView::Selection> selection = playlist.get_selection();
					TreeModel::Row row = *(selection->get_selected());
					actualTrack = row.get_value(playlist.columns.col_path);
					
					setButtonToPause();
					 
					actualTrackNumber = *(*selection->get_selected_rows().begin()).get_indices().begin();//playlist.getRowNumber(row);
					
					cerr << "onPlaylistDBLClick play " << actualTrackNumber << " " << actualTrack<< endl;
					
					player.play(actualTrack);
					set_title("aoide Musikplayer : "+actualTrack);
					time.set_range(0,player.getPlayLength()/1000.0);
					return true;
				}
        }
        else
                return false;
} 

bool main_window_glade::onTreeDoubleClick(GdkEventButton *event)
{
        static long unsigned lastclick = 0;

        cerr << "button press event detected" << endl;  

        if (event->type == GDK_2BUTTON_PRESS && event->button ==1)
        {
                if (lastclick == 0 || (event->time - lastclick) < 600)
                        lastclick = event->time;
                else
				{	
					RefPtr<Gtk::TreeView::Selection> selection = treeview.get_selection(); 
					struct stat entryStatus;
					string newPath =  treeview.getPath(selection->get_selected());
					stat(newPath.c_str(), &entryStatus);
					int entryType = entryStatus.st_mode & S_IFMT;
					cerr << "onTreeDoubleClick: entryType = " << entryType << endl;
					
					if(entryType == S_IFDIR)
					{
						cerr << "onTreeDoubleClick: dir found = " << newPath << endl;
						appendDirToPlaylist(newPath+"/");
					}
					else //file
					{
						cerr << "onTreeDoubleClick: file found" << endl;
						char *str1;
						str1 = strtok((char*)newPath.c_str(), "/");
						string str2 = str1;
						string path = "";
						while((str1 = strtok(NULL, "/"))!=NULL)
						{
							path += "/" + str2;
							str2 = str1;
						}
						path += "/" + str2;
						playlist.appendTitle(string(str2), path);
						cerr << "onTreeDoubleClick: str2 = " << str2 << "\t path = " << path << endl;
					}
					return true;
				}
        }
        else
                return false;
} 


void main_window_glade::setButtonToPlay()
{
	bPauseResume.set_image(*imPlay);
}

void main_window_glade::setButtonToPause()
{
	
	bPauseResume.set_image(*imPause);
}

void main_window_glade::savePlaylist(string path)
{
	playlist.save(path);
}

void main_window_glade::loadPlaylist(string path)
{
	playlist.load(path);
}

void main_window_glade::onAbout()
{
	cerr << "onAbout" << endl;
	Gtk::MessageDialog dialog(*this, "Aoide Musikplayer v. 1.0",
        false,MESSAGE_OTHER );
	dialog.set_secondary_text(
        "Autor: ny \n\n    gewidmet meinem Liebling \n\n                Tina");

	dialog.run();
}

main_window_glade::~main_window_glade()
{  	
	savePlaylist(playlistPath);
	delete imPause;
	delete imPlay; 
	delete imNext;
	delete imPrev;
	delete imStop;
	delete imRepTrack;
	delete imRepAll;
	delete imDelTrack;
	delete imDelAll;
	delete imAbout; 
	delete gmm_data;
}


