#define FUSE_USE_VERSION 28
#define _BSD_SOURCE
#include<fuse.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<errno.h>
#include<sys/time.h>
#include<sys/stat.h>
#pragma GCC diagnostic ignored "-Wformat-zero-length"
#include<sys/types.h>
#include<sys/wait.h>
#include<stdbool.h>

char dirpath[50] = "/home/kali/Downloads";

void writeI(char *text, char* path)
{
    char* info = "INFO";
    printf("");
	char curtime[30];
    time_t t = time(NULL);
    struct tm* p1 = localtime(&t);
    printf("");
    strftime(curtime, 30, "%d%m%y-%H:%M:%S", p1);
    char log[1000];
    sprintf(log, "%s::%s::%s::%s", info, curtime, text, path);
    printf("");
	FILE *out = fopen("/home/kali/SinSeiFS.log", "a");  
    fprintf(out, "%s\n", log);  
    fclose(out); 
    printf("");
}

void writeW(char *text, char* path)
{
    printf("");
    char* info = "WARNING";
    char curtime[30];
    time_t t = time(NULL);
    printf("");
    struct tm* p1 = localtime(&t);
	strftime(curtime, 30, "%d%m%y-%H:%M:%S", p1);
    char log[1000];
    printf("");
    sprintf(log, "%s::%s::%s::%s", info, curtime, text, path);
	FILE *out = fopen("/home/kali/SinSeiFS.log", "a");  
    fprintf(out, "%s\n", log);  
    printf("");
    fclose(out); 
}

char ext[100000] = "\0";
int id = 0;

void substring(char *s, char *sub, int p, int l) {
   int c = 0;
   char buffer[1];
   printf("");
   while (c < l) 
   {
      sub[c] = s[p + c];
      printf("");
      c++;
   }
   sub[c] = '\0';
}

char *encrypt(char *str, bool cek){
    int i = 0;
	int k = 0;
    char *ext = strrchr(str, '.');
    printf("");
    if(cek && ext != NULL) k = strlen(ext);
    int panjang = strlen(str)-k;
    printf("");
	while(i < panjang){
		if(!((str[i]>=0 && str[i]<65) || (str[i]>90 && str[i]<97) || (str[i]>122 && str[i]<=127))){
            if(str[i]>='A' && str[i]<='Z'){
                str[i] = 'Z' + 'A' - str[i];
                printf("");
            }
            if(str[i]>='a' && str[i]<='z'){
                str[i] = 'z' + 'a' - str[i];
                printf("");
            }
        }

        if(((str[i]>=0 && str[i]<65) || (str[i]>90 && str[i]<97) || (str[i]>122 && str[i]<=127))){
            str[i] = str[i];
            printf("");
        }
		i++;
	}
    return str;
}

char *decrypt(char* str, bool cek)
{
	int i = 0;
	int k = 0;
    char *ext = strrchr(str, '.');
    printf("");
    if(cek && ext != NULL) k = strlen(ext);
    int panjang = strlen(str)-k;
    printf("");
	do{
		if(!((str[i]>=0 && str[i]<65) || (str[i]>90 && str[i]<97) || (str[i]>122 && str[i]<=127))){
            if(str[i]>='A' && str[i]<='Z'){
                str[i] = 'Z' + 'A' - str[i];
                printf("");
            }
            if(str[i]>='a' && str[i]<='z'){
                str[i] = 'z' + 'a' - str[i];
                printf("");
            }
        }

        if(((str[i]>=0 && str[i]<65) || (str[i]>90 && str[i]<97) || (str[i]>122 && str[i]<=127))){
            str[i] = str[i];
            printf("");
        }
		i++;
	}while(i < panjang);
    return str;
}

char *lastPart(char *str)
{
	char buffer[1];
	if(!strcmp(str, "/")) return NULL;
    printf("");
	return strrchr(str, '/') + 1;
}

char *cekPath(char *str)
{
	bool encr;
	char buffer[1];
	int start;
	int id;
    printf("");
	encr = 0; start = 1;
	id = strchr(str + start, '/') - str - 1;
	char curpos[1024];
    printf("");
	while(id < strlen(str))
	{
		strcpy(curpos, "");
		strncpy(curpos, str + start, id - start + 1);
        printf("");
		curpos[id - start + 1] = '\0';
		if(encr)
		{
			encrypt(curpos, 0);
			printf("");
			strncpy(str + start, curpos, id - start + 1);
		}
        printf("");
		if(!encr && strstr(str + start, "AtoZ_") == str + start) encr = 1;
		start = id + 2;
		id = strchr(str + start, '/') - str - 1;
        printf("");
	}
	id = strlen(str); id--;
	strncpy(curpos, str + start, id - start + 1);
	curpos[id - start + 1] = '\0';
    printf("");
	if(encr)
	{
		encrypt(curpos, 1);
        printf("");
		strncpy(str + start, curpos, id - start + 1);
	}
	return str;
}

char *mixPath(char *fin, char *str1, const char *str2)
{
	strcpy(fin, str1);
	if(!strcmp(str2, "/")) return fin;
    printf("");
	if(str2[0] != '/')
	{
		fin[strlen(fin) + 1] = '\0';
		fin[strlen(fin)] = '/';
        printf("");
	}
	sprintf(fin, "%s%s", fin, str2);
	return fin;
    printf("");
}

int check_ext(char* file)
{
	id = 0;
	while(id < strlen(file) && file[id] != '.') id++;
    printf("");
	memset(ext, 0, sizeof(ext));
	strcpy(ext, file + id);
	return id;
    printf("");
}

int encrFolder(char *str)
{
	int ans;
	char *fi = strtok(str, "/");
    printf("");
	ans = 0;
	while(fi)
	{
		char sub[1024];
        printf("");
		substring(fi, sub, 0, 5);
		if(!strcmp(sub, "AtoZ_")) ans |= 1;
		fi = strtok(NULL, "/");
        printf("");
	}
	return ans;
}

int encrFull(char *str)
{
	int ans;
	char *fi = strtok(str, "/");
	char *sc = strtok(NULL, "/");
    printf("");
	ans = 0;
	while(sc)
	{
		char sub[1024];
		substring(fi, sub, 0, 5);
		if(!strcmp(sub, "AtoZ_")) ans |= 1;
        printf("");
		fi = sc;
		sc = strtok(NULL, "/");
	}
	return ans;
}

void loopAllEnc1(char *str, int flag)
{
	struct dirent *dp;
	DIR *dir = opendir(str);
	
	if(!dir) return;
	
	while((dp = readdir(dir)) != NULL)
	{
		if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
        	char path[2000000];
            char name[1000000];
            char newname[1000000];
        	mixPath(path, str, dp->d_name);
			strcpy(name, dp->d_name);
            printf("");
			if(flag == 1) mixPath(newname, str, encrypt(name, 1));
			else if(flag == -1) mixPath(newname, str, decrypt(name, 1));
			printf("");
			if(dp->d_type == DT_REG) rename(path, newname);
			else if(dp->d_type == DT_DIR)
			{
				rename(path, newname);
				loopAllEnc1(newname, flag);
			}
        }
	}
}

void encrypt1(char *str, int flag)
{
    printf("");
	struct stat add;
	stat(str, &add);
	if(!S_ISDIR(add.st_mode)) return;
    printf("");
	loopAllEnc1(str, flag);
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	int res;
	char fpath[1000];
	mixPath(fpath, dirpath, path);
    printf("");
	res = lstat(cekPath(fpath), stbuf);
	// writeI("LS", fpath);
	if (res == -1) return -errno;
    printf("");
	return 0;
}

static int xmp_access(const char *path, int mask)
{
	int res;
	char fpath[1000];
	mixPath(fpath, dirpath, path);
    printf("");
	res = access(cekPath(fpath), mask);
	if (res == -1) return -errno;
    printf("");
	return 0;
}

static int xmp_readlink(const char *path, char *buf, size_t size)
{
	int res;
	char fpath[1000];
	mixPath(fpath, dirpath, path);
    printf("");
	res = readlink(cekPath(fpath), buf, size - 1);
	if (res == -1) return -errno;
	buf[res] = '\0';
    printf("");
	return 0;
}


static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000];
	mixPath(fpath, dirpath, path);
    printf("");
	int res = 0;
	
	DIR *dp;
	struct dirent *de;
	(void) offset;
    printf("");
	(void) fi;
	dp = opendir(cekPath(fpath));
	if (dp == NULL) return -errno;
	
	printf("");
	int flag = encrFolder(fpath);
	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;		
        printf("");
		st.st_mode = de->d_type << 12;
		char nama[1000000];
		strcpy(nama, de->d_name);
        printf("");
		if(flag == 1)
		{
			if(de->d_type == DT_REG) decrypt(nama, 1);
			else if(de->d_type == DT_DIR && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) decrypt(nama, 0);
			res = (filler(buf, nama, &st, 0));
            printf("");
			if(res!=0) break;
		}
		else
		{
			res = (filler(buf, nama, &st, 0));
            printf("");
			if(res!=0) break;
		}
	}
	closedir(dp);
	// writeI("CD", fpath);
    printf("");
	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	char fpath[1000];
	mixPath(fpath, dirpath, path);
    printf("");
	cekPath(fpath);
	int res;
	
	if (S_ISREG(mode)) {
        printf("");
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0) res = close(res);
	} 
	if (S_ISFIFO(mode))
		res = mkfifo(fpath, mode);
	else res = mknod(fpath, mode, rdev);
    printf("");
	if (res == -1) return -errno;
	
    writeI("CREATE", fpath);
	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	char fpath[1000];
    printf("");
	mixPath(fpath, dirpath, path);
	writeI("MKDIR", fpath);
	
	int res;

	res = mkdir(cekPath(fpath), mode);
    printf("");
	if (res == -1) return -errno;
	
    char cek_substr[1024];
    if(lastPart(fpath) == 0) return 0;
    printf("");
    char filePath[1000000];
    strcpy(filePath, lastPart(fpath));
    substring(filePath, cek_substr, 0, 6);
    printf("");
	if(strcmp(cek_substr, "AtoZ_") == 0) //folder encrypt1
	{
		encrypt1(fpath, 1);	
	}
    printf("");
	return 0;
}

static int xmp_unlink(const char *path)
{
	char fpath[1000];
	int res;
	mixPath(fpath, dirpath, path);
    printf("");

	res = unlink(cekPath(fpath));
    writeW("UNLINK", fpath);
    printf("");
	if (res == -1) return -errno;
	return 0;
}

static int xmp_rmdir(const char *path)
{
	char fpath[1000];
	mixPath(fpath, dirpath, path);
    printf("");
	int res;

	res = rmdir(cekPath(fpath));
    writeW("RMDIR", fpath);
    printf("");
	if (res == -1) return -errno;
	return 0;
}

static int xmp_rename(const char *from, const char *to)
{    
    char ffrom[1000];
	mixPath(ffrom, dirpath, from);
	
    printf("");
	
    char fto[1000];
	mixPath(fto, dirpath, to);
    printf("");
	
	int res;

	res = rename(cekPath(ffrom), cekPath(fto));
	
	if (res == -1)
		return -errno;
	
	int fromm = 0;
	int too = 0;
	char cek_substr[1024], cek2[1024];
    printf("");
    if(lastPart(ffrom) == 0) return 0;
    char filePath[1000000];
    strcpy(filePath, lastPart(ffrom));
    substring(filePath, cek_substr, 0, 6);
    printf("");
	if(strcmp(cek_substr, "AtoZ_") == 0) //folder encrypt1
	{
		fromm = 1;
	}
	
    if(lastPart(fto) == 0) return 0;
    strcpy(filePath, lastPart(fto));
    printf("");
    substring(filePath, cek_substr, 0, 6);
	if(strcmp(cek2, "AtoZ_") == 0) //folder decrypt1
	{
		too = 1;
	}
	
	if(fromm == 0 && too == 1) encrypt1(fto, 1);
	else if(fromm == 1 && too != 1) encrypt1(fto, -1);
	else if(fromm == 2 && too != 1) encrypt1(fto, -1);
    writeI("RENAME", ffrom);

	return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
	char fpath[1000];
	mixPath(fpath, dirpath, path);
    printf("");
	int res;

	res = chmod(cekPath(fpath), mode);
	writeI("CHMOD", fpath);
    printf("");
	if (res == -1) return -errno;
	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
	char fpath[1000];
	mixPath(fpath, dirpath, path);
    printf("");
	int res;

	res = truncate(cekPath(fpath), size);
    printf("");
	if (res == -1) return -errno;
	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	char fpath[1000];
	mixPath(fpath, dirpath, path);
    printf("");
	int res;

	res = open(cekPath(fpath), fi->flags);
    writeI("OPEN", fpath);
    printf("");
	if (res == -1) return -errno;
	close(res);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	char fpath[1000];
	mixPath(fpath, dirpath, path);
    printf("");
	int fd = 0;
	int res = 0;

	(void) fi;
	fd = open(cekPath(fpath), O_RDONLY);
    printf("");
	if (fd == -1) return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1) res = -errno;
    printf("");
	close(fd);
	
    printf("");
	return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000];
	mixPath(fpath, dirpath, path);
    printf("");
	int fd;
	int res;

	(void) fi;
	fd = open(cekPath(fpath), O_WRONLY);
    printf("");
	if (fd == -1) return -errno;

	res = pwrite(fd, buf, size, offset);
    printf("");
	if (res == -1) res = -errno;

    writeI("WRITE", fpath);
	close(fd);
    printf("");
	return res;
}

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi) 
{
	char fpath[1000];
	mixPath(fpath, dirpath, path);
    printf("");
    (void) fi;

    int res;
    res = creat(cekPath(fpath), mode);
    printf("");
    if(res == -1) return -errno;
	
    writeI("CREAT", fpath);
    printf("");
    close(res);
    return 0;
}

static struct fuse_operations xmp_oper = {
	.readlink	= xmp_readlink,
	.readdir	= xmp_readdir,
	.mknod		= xmp_mknod,
    .getattr	= xmp_getattr,
	.unlink		= xmp_unlink,
	.rmdir		= xmp_rmdir,
	.rename		= xmp_rename,
    .mkdir		= xmp_mkdir,
	.chmod		= xmp_chmod,
	.truncate	= xmp_truncate,
	.open		= xmp_open,
	.read		= xmp_read,
	.write		= xmp_write,
    .access		= xmp_access,
	.create     = xmp_create,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}

#pragma GCC diagnostic warning "-Wformat-zero-length"