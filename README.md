# dircpmvlist
A C program dircpmvlist that copies or moves a directory tree rooted at a specific path in the home directory to a specific destination folder in the home directory along the file types specified in the extension list (or the entire directory if the extension list is not specified)

# Synopsis :
# dircpmvlist [ source_dir] [destination_dir] [options] <extension list>
 Both source_dir and destination_dir can be either absolute or relative paths but must belong to the home directory hierarchy.
 If the destination_dir is not present in the home directory hierarchy, it should be newly created.
 options
o -cp copy the directory rooted at source_dir to destination_dir and do not delete
the directory (and contents) rooted at source_dir
o -mv move the directory rooted at source_dir to destination_dir and delete the
directory (and contents) rooted at source_dir
 extension list: up to 6 file extensions can be provided ( c , pdf, txt etc.)
o If the extension list is provided with -cp:
 The entire sub-tree rooted at source_dir along with all its folders/subfolders
and the corresponding file types (only) listed in the extension list
must be copied onto the destination_dir.
 All the folders/sub-folders/files must be copied onto destination_dir as per
the original hierarchy at source_dir.
 File types not listed in the extension list must not be copied to the
destination dir
 If desintation_dir does not exist, it must be created.
o If the extension list is provided with -mv:
 The entire sub-tree rooted at source_dir along with all its folders/subfolders
and the corresponding file types (only) listed in the extension list
must be copied onto the destination_dir.
 All the folders/sub-folders/files must be copied onto destination_dir as per
the original hierarchy at source_dir.
 File types not listed in the extension list must not be copied to the
destination dir
 If desintation_dir does not exist, it must be created.
 The original subtree rooted at source_dir must be deleted entirely along
with its folders/sub-folders/files (All files) etc.
o If the extension list is not provided, all files and folders must be copied or moved
as per the option chosen. (Very Important!!)
