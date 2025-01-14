/* 
* Descent 3 
* Copyright (C) 2024 Parallax Software
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
* $Logfile: /DescentIII/Main/Lib/unzip.h $
* $Revision: 3 $
* $Date: 8/15/99 8:07p $
* $Author: Jeff $
*
* Unzip class
*
* $Log: /DescentIII/Main/Lib/unzip.h $
 * 
 * 3     8/15/99 8:07p Jeff
 * handle disk write errors when extracting
 * 
 * 2     8/13/99 8:01p Jeff
 * initial creation of zip class
*
* $NoKeywords: $
*/



#ifndef __UNZIP_H
#define __UNZIP_H

#include <stdio.h>
#include "pstypes.h"

typedef struct
{
	uint	cent_file_header_sig;
	ubyte	version_made_by;
	ubyte	host_os;
	ubyte	version_needed_to_extract;
	ubyte	os_needed_to_extract;
	ushort	general_purpose_bit_flag;
	ushort	compression_method;
	ushort	last_mod_file_time;
	ushort	last_mod_file_date;
	uint	crc32;
	uint	compressed_size;
	uint	uncompressed_size;
	ushort	filename_length;
	ushort	extra_field_length;
	ushort	file_comment_length;
	ushort	disk_number_start;
	ushort	internal_file_attrib;
	uint	external_file_attrib;
	uint	offset_lcl_hdr_frm_frst_disk;
	char*   name;
}zipentry;

class ZIP
{
public:
	//constructor/destructor
	ZIP();
	~ZIP();

public:
	//opens a zip file for reading
	//returns true on success
	bool OpenZip(const char *path);

	//closes an open zip file
	void CloseZip(void);

	// Reads the current zip entry from the zip file (and moves
	// to the next entry).  Returns NULL if there are no more entries
	zipentry* ReadNextZipEntry(void);

	// Resets a ZIP file to the first entry
	void Rewind(void);

	// Reads a file from the given zip entry into a buffer in memory
	//	-1 : no ZIP file open
	//	-2 : corrupt file
	//	-3 : Version too new
	//	-4 : OS not supported
	//	-5 : No Disk Spanning
	//	-6 : Error inflating
	//	-7 : Compression Type Not Supported
	int ReadFile(zipentry *ent,char *data);

	//	Extracts a file from the given zip entry into another file
	//	-1 : no ZIP file open
	//	-2 : corrupt file
	//	-3 : Version too new
	//	-4 : OS not supported
	//	-5 : No Disk Spanning
	//	-6 : Error inflating
	//	-7 : Compression Type Not Supported
	//	-8 : Unable to open output
	//	-9 : Error writing to file
	int ExtractFile(zipentry *ent,const char *filename);

private:
	bool FindECDSignature(char *buffer,int buflen,int *offset);
	int ReadECD(void);
	int ReadZipData(zipentry* ent,char* data);
	int ReadZipDataToFile(zipentry* ent,FILE *file);
	int SeekToCompressedData(zipentry* ent);	
	int InflateFile(FILE* in_file,unsigned in_size,ubyte* out_data,unsigned out_size);
	int InflateFileToFile(FILE* in_file,unsigned in_size,FILE *file,unsigned out_size);
private:
	bool m_open;
	char* m_zip;		// zip name
	FILE* m_fp;			// zip handler
	long m_length;		// length of zip file

	char* m_ecd;		// end_of_cent_dir data
	unsigned m_ecd_length;// end_of_cent_dir length

	char* m_cd;			// cent_dir data

	unsigned m_cd_pos;	// position in cent_dir

	zipentry m_ent;		// buffer for readzip 

	// end_of_cent_dir
	uint	m_end_of_cent_dir_sig;
	ushort	m_number_of_this_disk;
	ushort	m_number_of_disk_start_cent_dir;
	ushort	m_total_entries_cent_dir_this_disk;
	ushort	m_total_entries_cent_dir;
	uint	m_size_of_cent_dir;
	uint	m_offset_to_start_of_cent_dir;
	ushort	m_zipfile_comment_length;
	char*	m_zipfile_comment;	// pointer in ecd
};

// Compare two filename without using directory
int CompareZipFileName(const char* zipfile, const char* file);

#endif
