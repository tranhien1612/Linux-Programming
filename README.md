# Linux-Programming

## BSP

Bộ BSP: đóng vai trò kết nối phần cứng với phần mềm

- **Bootloader**: Bộ nạp khởi động
- **Linux Kernel**: Thành phần quan trọng nhất của hệ thống, chứa bộ lập lịch các tiến trình, quản lý bộ nhớ, quản lý thiết bị…
- **Rootfs**: Hệ thống file system.
- **Toolchain**: Bao gồm các thành phần chính sau:
    - binutils (Công cụ liên quan đến xử lý mã nhị phân): GNU Assembler, Linker, …
    - gcc: GNU C Complier
    - C library (libc)
    - gdb: Debugger
    
    Có 2 loại toolchain: 
    
    - Native: Chạy trên máy nào sinh mã nhị phân chạy cho máy đó.
    - Cross: Chạy trên 1 máy nhưng sinh mã nhị phân cho một máy khác.

Các công việc liên quan đến bộ BSP:

- Bootloader: Tối ưu thời gian khởi động, thêm cmd điều khiển, phân vùng flash, …
- Kernel: Viết driver I2C, SPI, ….
- Rootfs: Phát triển ứng dụng trên tầng user space

## Process

Program: Là một nhóm các câu lệnh để thực thi một nhiệm vụ cụ thể được thể hiện bằng các file thực thi và nằm trên ổ cứng máy tính.

Process: Là một chương trình đang được thực thi và sử dụng tài nguyên của hệ thống. Mỗi tiến trình được đại diện bằng 1 PID duy nhất.

Kiểm tra các tiến trình (process):

```
ps a
```

Memory Layout của một Process:

- Text segment: Chứa mã máy (code)
- Initialized data segment: Chứa các biến global, static đã được khởi tạo 1 cách tường minh (đã được gán giá trị).
- Uninitialized data segment: Chứa các biến global, static đã được khởi tạo không tường minh (chưa được gán giá trị).
- Stack: Chứa các biến cục bộ
- Heap: Cấp phát bộ nhớ động.

Thao tác với Process:

- Tạo 1 tiến trình mới:
    - Sử dụng system call fork()
- Chạy 1 chương trình trong 1 chương trình:
    - Sử dụng hàm thuộc dòng exec (execle, execlp, …)
- Kết thúc tiến trình:
    - Dùng hàm exit() hoặc _exit().

## Thread

Thread được tạo ra nhằm mục đích xử lý đồng thời nhiều công việc trong cùng một thời điểm (multi-task).

Nguyên lý hoạt động:

- Trên một hệ thống multi-core, nhiều thread có thể hoạt động song song.
- Nếu một thread bị block thì các thread khác vẫn hoạt động bình thường.
- Mỗi khi tạo một thread, chúng sẽ được lưu trữ trong stack segment.

## IPC

IPC (InterProcess Communication) là các phương thức được sử dụng để giao tiếp giữa 2 process (chia sẻ dữ liệu và đồng bộ truy cập)

```
--- IPC
    --- Communication
            Data Tranfer
                ByteStream
                    pipe
                    FIFO
                    Stream Socket
                Message
                    Queue
                    Datagram Socket
            Share Memory
    --- Synchronization
            Semaphore
            File Lock
            Mutex
            Conditional Variable
```

Linux cung cấp một số cơ chế giao tiếp giữa các tiến trình gọi là IPC (Inter-Process Communication):

- Signals handling - Trao đổi bằng tín hiệu
- Pipe -Trao đổi bằng cơ chế đường ống
- Message Queues - Trao đổi thông qua hàng đợi tin nhắn
- Shared Memory - Trao đổi bằng phân đoạn nhớ chung
- Giao tiếp thông qua socket
- Giao tiếp đồng bộ dùng semaphore

[tldp.org/LDP/tlk/ipc/ipc.html](https://tldp.org/LDP/tlk/ipc/ipc.html)

## Signal

Là một trong những phương thức giao tiếp liên tiến trình. Được sử dụng ddeerr báo hiệu các tín hiệu không đồng bộ đến một hoặc nhiều process.

```
# Show signal
kill -l
 1) SIGHUP	 2) SIGINT	 3) SIGQUIT	 4) SIGILL	 5) SIGTRAP
 6) SIGABRT	 7) SIGBUS	 8) SIGFPE	 9) SIGKILL	10) SIGUSR1
11) SIGSEGV	12) SIGUSR2	13) SIGPIPE	14) SIGALRM	15) SIGTERM
16) SIGSTKFLT	17) SIGCHLD	18) SIGCONT	19) SIGSTOP	20) SIGTSTP
21) SIGTTIN	22) SIGTTOU	23) SIGURG	24) SIGXCPU	25) SIGXFSZ
26) SIGVTALRM	27) SIGPROF	28) SIGWINCH	29) SIGIO	30) SIGPWR
31) SIGSYS	34) SIGRTMIN	35) SIGRTMIN+1	36) SIGRTMIN+2	37) SIGRTMIN+3
38) SIGRTMIN+4	39) SIGRTMIN+5	40) SIGRTMIN+6	41) SIGRTMIN+7	42) SIGRTMIN+8
43) SIGRTMIN+9	44) SIGRTMIN+10	45) SIGRTMIN+11	46) SIGRTMIN+12	47) SIGRTMIN+13
48) SIGRTMIN+14	49) SIGRTMIN+15	50) SIGRTMAX-14	51) SIGRTMAX-13	52) SIGRTMAX-12
53) SIGRTMAX-11	54) SIGRTMAX-10	55) SIGRTMAX-9	56) SIGRTMAX-8	57) SIGRTMAX-7
58) SIGRTMAX-6	59) SIGRTMAX-5	60) SIGRTMAX-4	61) SIGRTMAX-3	62) SIGRTMAX-2
63) SIGRTMAX-1	64) SIGRTMAX

# Send signal to process (PID)
kill -<signal> <PID>
```

## Pipe

- Pipe thường được sử dụng trong giao tiếp giữa process cha và process con bằng 1 visual file.
- Pipe (đường ống) là kênh truyền dữ liệu giữa các tiến trình với nhau, dữ liệu xuất của tiến trình này được chuyển đến làm dữ liệu nhập cho tiến trình kia dưới dạng một dòng các byte.
- Trên đường ống dữ liệu chỉ có thể chuyển đi theo một chiều, dữ liệu vào đường ống tương đương với thao tác ghi (pipe write), lấy dữ liệu từ đường ống tương đương với thao tác đọc (pipe read), một tiến trình kết nối với một pipe chỉ có thể thực hiện một trong hai thao tác đọc hoặc ghi, nhưng không thể thực hiện cả hai.
- Có hai loại pipe:
    - **Unnamed pipe**: có ý nghĩa cục bộ, dành cho các tiến trình có quan hệ cha con
    - **Named pipe (còn gọi là FIFO)**: có ý nghĩa toàn cục, sử dụng cho các tiến trình không có quan hệ cha con.

```
# Create pipe: filedes[0]: đọc và filedes[1]: ghi.
int pipe(int filedes[2])

# Read, write data in pipe
write(int fd, const void *buf, size_t count)
read(int fd, const void *buf, size_t count)

int fifledes[2]; //mang chứa file descriptors
pipe(fifledes); //tạo pipe
```

## Shared Memory

Shared Memory là một vùng nhớ cho phép nhiều tiến trình có thể truy cập đến. Có tác dụng làm tăng tốc độ xử lý giữa các tiến trình. Khi shared memory được tạo nó sẽ tạo ra 1 vùng nhớ ảo nằm giữa phân vùng head và stack.

Có 2 phương pháp để tạo shared memory:

- Sử dụng IPA của shm (System V)
- Sử dụng IPA của mmap (POSIX)

## Message Queue

***Message queue*** hay ***hàng đợi tin nhắn*** cũng là một cơ chế IPC có sẵn trong Linux. Mỗi một khối dữ liệu được truyền đi được xác định một kiểu (TYPE) cụ thể và người nhận có thể nhận được các dữ liệu đó tùy theo kiểu của dữ liệu. 

## Semaphore

Semaphore: là cơ chế đồng bộ truy cập giữa các process và thread. Semaphore cung cấp 2 hoạt động tăng/giảm giá trị của semaphore. Nếu giá trị semaphore = 0 thì tiến trình nào đang thực hiện việc wait (giảm semaphore) sẽ bị block cho đến khi giá trị semaphore > 0.

Có 2 loại semaphore:

- Semaphore sử dụng API của System V
- Semaphore sử dụng API của POSIX
    - Named Semaphore: Được lưu trữ bởi 1 file được lưu trữ trong thư mục /dev/shm
    - Unnamed Semaphore: Được lưu trữ bởi 1 vùng nhớ
