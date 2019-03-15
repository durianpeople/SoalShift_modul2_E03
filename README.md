# Laporan

## SOAL 1

Melakukan pembuatan daemon, namun chdir ke folder **modul2**:

```c
if ((chdir("/home/<user>/modul2/")) < 0)
{
    exit(EXIT_FAILURE);
}
```

Membuka folder saat ini (.), lalu melakukan iterasi untuk setiap file dalam folder tersebut:

```c
DIR *dr = opendir(".");
if (dr == NULL)
{
    // printf("Gagal buka direktori\n");
    break;
}
while ((de = readdir(dr)) != NULL)
{
    //...
}
```

Melakukan fork

```c
child_id = fork();
```

Child memastikan bahwa file yang sedang diiterasi memiliki ekstensi **.png**. Jika iya, pindah ke dalam folder **gambar** lalu exit:

```c
if (child_id == 0)
{
    // child
    char *ext = strstr(de->d_name, ".png");
    int stringlength = strlen(de->d_name);
    int pointer = (ext - de->d_name);
    int trueext = (strcmp(ext, ".png")==0);
    if (trueext)
    {
        // printf("True Ext\n");
        char basename[pointer + 1];
        memcpy(basename, &de->d_name[0], pointer);
        basename[pointer] = '\0';
        // https://stackoverflow.com/questions/7430248/creating-a-new-directory-in-c
        struct stat st = {0};
        if (stat("gambar", &st) == -1)
        {
            mkdir("gambar", 0777);
        }
        char target[1000] = "./gambar/";
        strcat(target, basename);
        strcat(target, "_grey.png");
        // char* target=strcat(basename, "_grey.png");
        // printf("Execv %s to %s\n", de->d_name, target);
        rename(de->d_name, target);
    }
    exit(0);
}
```

Parent akan menunggu child hingga selesai mengecek dan/atau memindahkan file, lalu melanjutkan ke file selanjutnya:

```c
else
{
    // parent
    while (wait(&status) > 0);
    continue;
}
```

Menutup folder yang dibuka tadi (/home/<user>/modul2), lalu sleep:

```c
closedir(dr);
sleep(10);
```

## SOAL 2

Mengambil owner dan grup dari file hatiku/elen.ku

```c
struct stat info;
stat("hatiku/elen.ku", &info);
struct passwd *pw = getpwuid(info.st_uid);
struct group *gr = getgrgid(info.st_gid);
```

Memastikan jika owner dan grup adalah **www-data**:

```c
if (pw != 0 && gr != 0)
{
    if (strcmp(pw->pw_name, "www-data") == 0 && strcmp(gr->gr_name, "www-data") == 0)
    {
        //..
    }
}
```

Mengubah mode file menjadi **777** kemudian menghapus file tersebut

```c
if (chmod("hatiku/elen.ku", S_IRWXG | S_IRWXO | S_IRWXU) == 0)
{
    remove("hatiku/elen.ku");
}
```

## SOAL 3

Extract file dengan unzip 

```c
char *argv[3] = {"unzip", "/home/akmalfakhrib/Documents/sisop/SoalShift_modul2_E03-back/campur2.zip", NULL};
execv("/usr/bin/unzip", argv);
```

lalu list file dengan fungsi ls, tapi fork process unzip sehingga process ls merupakan child process dari process unzip
```c
char *argv[3] = {"ls", "/home/akmalfakhrib/Documents/sisop/SoalShift_modul2_E03-back/campur2", NULL};
      execv("/bin/ls", argv);
```

lalu bikin file text.txt yang nanti nya akan dijadikan tempat untuk menyimpan list file hasil unzip
```c
char *argv[3] = {"touch", "/home/akmalfakhrib/Documents/sisop/SoalShift_modul2_E03-back/text.txt", NULL};
execv("/usr/bin/touch", argv);
```

lalu lakukan grep hasil list file, dengan ketentuan hanya file berextensi .txt
```c
char *argv[3] = {"grep", "[.]txt$", NULL};
execv("/bin/grep", argv);
```

## SOAL 4

Ganti directory

```c
if ((chdir("/home/durianpeople/Documents/makanan")) < 0)
    {
        exit(EXIT_FAILURE);
    }
```

Mengambil waktu terakhir kali diakses (dalam detik):

```c
struct stat filestat;
stat("makan_enak.txt", &filestat);

time_t current_time = time(NULL);
time_t diff = difftime(current_time, filestat.st_atime);
```

Jika waktu kurang dari atau sama dengan 30 detik, maka membuat file **makan_sehat[counter].txt**, dengan counter dimulai dari 1:

```c
char target[1000];
sprintf(target, "makan_sehat%d.txt", counter++);
FILE *ftarget = fopen(target, "w+");
fclose(ftarget);
```

## SOAL 5

### A

Mengubah direktori ke folder **log**:

```c
if ((chdir("/home/<user>/log/")) < 0)
{
    exit(EXIT_FAILURE);
}
```

Setelah **fork**, child menyimpan **pid**-nya ke sebuah file **pid** di dalam folder **log**:

```c
FILE *piddaemon = fopen("pid", "w+");
fprintf(piddaemon, "%d", getpid());
fclose(piddaemon);
while (1)
{
    int tmp = program(); //program utama
    sleep(0);
}
```

Di dalam `program()`:

Mengambil waktu saat ini, kemudian menyimpannya ke dalam variabel **foldername** dengan format yang telah ditentukan:

```c
time_t t = time(NULL);
struct tm tm = *localtime(&t);
char foldername[1000];
strftime(foldername, 1000, "%d:%m:Y-%H:%M", &tm);
```

Membuat folder dengan nama format yang telah ditentukan:

```c
struct stat st = {0};
if (stat(foldername, &st) == -1)
{
    mkdir(foldername, 0777);
}
```

Iterasi 30 kali, dengan setiap iterasi 1 menit:

```c
int counter = 0;
while (counter++ < 30)
{
    //..
    sleep(60);
}
```

Mengubah integer `counter` menjadi sebuah string untuk kemudian ditambahkan untuk nama file target:

```c
char counter_string[1000];
itoa(counter, counter_string);
```

Membuka file **/var/log/syslog** untuk dibaca:

```c
FILE *source = fopen("/var/log/syslog", "r");
if (source == NULL)
    printf("Gagal 1\n"), exit(0);
```

Membuat string **filename** dengan isi nama file log target yang disesuaikan dengan format yang telah ditentukan, lalu dibuka untuk ditulis (jika tidak ada akan dibuat secara otomatis):

```c
char filename[1000] = "/home/<user>/log/";
strcat(filename, foldername);
strcat(filename, "/log");
strcat(filename, counter_string);
strcat(filename, ".log");
printf("%s\n", filename);
FILE *dest = fopen(filename, "a+");
if (dest == NULL)
    printf("Gagal 2\n"), exit(0);
```

Membaca setiap karakter dari **/var/log/syslog**, kemudian menulisnya ke dalam file log target:

```c
char pointer;
pointer = fgetc(source);
while (pointer != EOF)
{
    fputc(pointer, dest);
    pointer = fgetc(source);
}
fclose(source);
fclose(dest);
```

### B

Pindah direktori ke folder **log**:

```c
if ((chdir("/home/<user>/log/")) < 0)
{
    exit(EXIT_FAILURE);
}
```

Mengambil PID dari file **pid**:

```c
FILE *piddaemon = fopen("pid", "r");
char pids[100];
fgets(pids, 100, piddaemon);
fclose(piddaemon);
```

Mengubah `pids	` menjadi tipe **integer**, kemudian mengirimkan **SIGKILL** ke PID tersebut:

```c
int pid = atoi(pids);
kill(pid, SIGKILL);
```

