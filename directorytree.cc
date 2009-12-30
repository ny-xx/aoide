/*
  	Filename: .cc
	Author: Kreil Dominik	
	Description:

	Aoide is a simple musicplayer using the file structure
*/
#include "directorytree.hh"



DirectoryTree::DirectoryTree(string *rPath) : rootPath(rPath)
{
	signal_row_expanded().connect_notify(sigc::mem_fun(*this, &DirectoryTree::onExpanded));
	signal_row_collapsed().connect_notify(sigc::mem_fun(*this, &DirectoryTree::onCollapsed));


	fileTypes.push_back("mp3");
	fileTypes.push_back("ogg");
	fileTypes.push_back("wav");
	fileTypes.push_back("mod");
	fileTypes.push_back("xm");
	fileTypes.push_back("it");
	fileTypes.push_back("s3m");

	int size = 20;
	imOpen = new Image(Image("images/mfopen.png").get_pixbuf()->scale_simple(size,size,Gdk::INTERP_BILINEAR));
	imClosed = new Image(Image("images/mfclosed.png").get_pixbuf()->scale_simple(size,size,Gdk::INTERP_BILINEAR));
	size = 25;
	imMusic = new Image(Image("images/sound_icon.png").get_pixbuf()->scale_simple(size,size,Gdk::INTERP_BILINEAR));

	set_model(treeStore = TreeStore::create(columns)); 
	

	append_column("", columns.col_img); 
	append_column("name.", columns.col_name); 

	TreeModel::Row row = *(treeStore->append());
	row[columns.col_name] = rootPath->c_str();
	row[columns.col_img] = imClosed->get_pixbuf();
	row = *(treeStore->append(row->children())); 
	row[columns.col_name] = "";
}

string DirectoryTree::getPath(const Gtk::TreeModel::iterator & iter)
{
	vector<string> inversePath;

	Gtk::TreeModel::iterator path_iter = iter;
	while(path_iter!=NULL)
	{	
		log<<path_iter->get_value(columns.col_name);
		inversePath.push_back(path_iter->get_value(columns.col_name));
		path_iter = path_iter->parent();
	}

	//reverse(inversePath.begin(), inversePath.end());
	vector<string>::iterator invIter = inversePath.end();	
	string hardPath = "";
	hardPath += *(--invIter);
	while(invIter!=inversePath.begin())
	{
		hardPath += '/' + *(--invIter) ;
		log << hardPath;
	}	
	return hardPath;
}


void DirectoryTree::onExpanded(const Gtk::TreeModel::iterator & iter,const Gtk::TreeModel::Path & path)
{
	iter->set_value(columns.col_img, imOpen->get_pixbuf());
	
	if(iter->children().size()>1) return;

	makeSubtree((TreeModel::Row&)(*iter), getPath(iter));
	treeStore->erase(iter->children().begin());
}

void DirectoryTree::onCollapsed(const Gtk::TreeModel::iterator & iter,const Gtk::TreeModel::Path & path)
{
	iter->set_value(columns.col_img, imClosed->get_pixbuf());
}



bool DirectoryTree::validateFileType(string path)
{
	string path2 = path;
	char *str1;
	str1 = strtok((char*)path2.c_str(), ".");
	char *str2=str1;
	while((str1 = strtok(NULL, "./"))!=NULL)
		str2 = str1;
	vector<string>::iterator iter = fileTypes.begin();
	for(iter; iter!=fileTypes.end(); iter++)
		if (iter->c_str() == (string)str2)
			return true;
	return false; 	
}

int DirectoryTree::makeSubtree(TreeModel::Row &parrent, string path)
{
	DIR *hdir;
	struct dirent *entry;
	hdir = opendir(path.c_str());
	if (hdir == NULL)
	{
		cout << "error while opening " << path << endl;
		return 0;
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
			string newPath = path + (string)("/") + d_name;
			stat(newPath.c_str(), &entryStatus);
			int entryType = entryStatus.st_mode & S_IFMT;
			switch (entryType) {
				case S_IFREG: 	if(validateFileType(newPath.c_str()))
								{
									log<<"search: " << d_name;
									static TreeModel::Row row;
									row = *(treeStore->append(parrent.children())); 
									row[columns.col_name] = d_name;
									row[columns.col_img] = imMusic->get_pixbuf();
								}
								break;
				case S_IFLNK: //puts("Symbolischer Link"); 
								break;
				case S_IFDIR: //cout << "Verzeichnis: " << newPath << endl;
//								read(newPath, lebParrent);
								log<<"search: " << d_name;
								static TreeModel::Row row;
								row = *(treeStore->append(parrent.children())); 
								row[columns.col_name] = d_name;
								row[columns.col_img] = imClosed->get_pixbuf();
								row = *(treeStore->append(row.children())); 
								row[columns.col_name] = "";
								break;
				default: 		//puts("Sonstiges");
								break;
			}
			//cout << entry->d_name << endl;
			//delete[] newPath;
		}
	} while (entry);
	closedir(hdir);	
	return EXIT_SUCCESS;
}

DirectoryTree::~DirectoryTree()
{
	delete imOpen;
	delete imClosed;
	delete imMusic;
}


