//
// C++ Implementation: playlist 
//
// Description: 
// 
//
// Author: ny,,, <ny.softwared@gmx.at>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "playlist.hh"

Playlist::Playlist()
{
//	set_title("Playlist"); 
	numberOfTracks = 0;
	set_model(listStore = Gtk::ListStore::create(columns)); 
	set_reorderable();							//drag and drop
	append_column("", columns.col_img); 
	append_column("title", columns.col_title); 
	append_column("path", columns.col_path); 
	
	int size = 25;
	imMusic = new Image(Image("images/sound_icon.png").get_pixbuf()->scale_simple(size,size,Gdk::INTERP_BILINEAR));

//	append_column("path" , columns.col_path);
}


void Playlist::appendTitle(string title, string path)
{
	Gtk::TreeModel::Row row = *(listStore->append());
    row[columns.col_path] = path;
	row[columns.col_title] = title;
	row[columns.col_img] = imMusic->get_pixbuf();
	numberOfTracks++; 
}

Playlist::~Playlist()
{
	delete imMusic;
}

//obsolet
string Playlist::getRowPath(int n)
{
	TreeModel::Children children = listStore->children();
	TreeModel::Children::iterator iter = children.begin();
	for(int i=0; i<n; i++)
	{
		if(!iter) return "";
		if(iter++ == children.end())
			return "";
	}
	return iter->get_value(columns.col_path);
}



int Playlist::getRowNumber(TreeModel::Row &row)
{
	TreeModel::Children children = listStore->children();
	int i = 0;
	for(TreeModel::Children::iterator iter = children.begin(); iter != children.end(); ++iter)
	{
		i++;
		cerr << "getRowNumber \t " << i << " " << iter->get_value(columns.col_title)<<endl;
		if(iter.equal(row))
			return i;
	}
	return 0;
}

void Playlist::save(string path)
{
	ofstream out(path.c_str());
	TreeModel::Children children = listStore->children();
	TreeModel::Children::iterator iter = children.begin(); 
	while(iter != children.end())
	{
		out << iter->get_value(columns.col_title) << "\t" << iter->get_value(columns.col_path) << endl;
		iter++;
	};
	out.close();
}

void Playlist::load(string path)
{
	/*ifstream in(path.c_str());
	string n_title, n_path;
	
	while(!in.eof())
	{
		in >> n_title ;
		if(in.eof()) break;
		in >> n_path;
		appendTitle(n_title, n_path);
		cout << "Playlist::load " << n_title << "\t" << n_path << endl; 
	}
	in.close();*/
}
