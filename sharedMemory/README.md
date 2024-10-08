# memory page structure
Library support
```
	shm API
	mmap
```

## shm API
Sequence diagram to share memory via shm API

generate a unique key from string
```
	#include <sys/types.h>
	#include <sys/ipc.h>
	key_t ftok(const char *pathname, int proj_id); //convert a pathname and a project identifier to a key
```

Alloc memory from key
```
	int shmget(key_t key, size_t size, int shmflg); //Allocate and return memory address for share memory
	//IPC_CREAT
	//PROT_READ | PROT_WRITE
	//Permission like a file
```
Map shared memory to process address space
```
	void *shmat(int shmid, const void *shmaddr, int shmflg); //shmaddr should be NULL
	//SHM_RDONLY
```
Unmap shared memory in process address space
```
	int shmdt(const void *shmaddr);
```
Destroy shared memory
```
	int shmctl(int shmid, int cmd, struct shmid_ds *buf); //IPC_RMID
```

## mmap API
Sequence diagram to share memory via mmap API

function API
```
	#include <sys/mman.h>
	void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
	int munmap(void *addr, size_t length);
	shm_open(name, O_CREAT | O_RDRW, 0666); 
	ftruncate(shm_fd, SIZE);
```
