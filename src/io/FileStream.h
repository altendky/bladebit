#pragma once

enum class FileAccess : uint16
{
    None  = 0,
    Read  = 1 << 0,
    Write = 1 << 1,
    ReadWrite = Read | Write
};
ImplementFlagOps( FileAccess );

enum class FileMode : uint16
{
    Open   = 0,
    Create = 1,
    Append = 2
};

enum class FileFlags : uint32
{
    None        = 0,
    NoBuffering = 1 << 0,
    LargeFile   = 1 << 1,
};
ImplementFlagOps( FileFlags );


enum class SeekOrigin : int32
{
    Begin  = 0,
    Current,
    End
};

class FileStream
{
public:
    inline FileStream() {}
    inline ~FileStream()
    {
        Close();
    }

    // Duplicate a file
    // FileStream( const FileStream& src );

    // void WriteAsync( byte* buffer, size_t size );

    static bool Open( const char* path, FileStream& file, FileMode mode, FileAccess access, FileFlags flags = FileFlags::None );
    bool Open( const char* path, FileMode mode, FileAccess access, FileFlags flags = FileFlags::None );

    ssize_t Read( void* buffer, size_t size );
    ssize_t Write( const void* buffer, size_t size );

    bool Reserve( ssize_t size );
    
    bool Seek( int64 offset, SeekOrigin origin );

    bool Flush();

    inline size_t BlockSize() { return _blockSize; }

    void Close();

    bool IsOpen() const;
    static bool Exists( const char* path );

    inline int GetError()
    {
        int err = _error;
        _error = 0;

        return err;
    }


private:
    size_t     _writePosition = 0;
    size_t     _readPosition  = 0;
    FileAccess _access        = FileAccess::None;
    FileFlags  _flags         = FileFlags::None;
    int        _error         = 0;

    #if PLATFORM_IS_UNIX
        int    _fd = -1;
        size_t _blockSize   = 0;        // for O_DIRECT
        // byte*  _blockBuffer = nullptr;  // for O_DIRECT
        // uint32 _blockBytes  = 0;        // Pending block-unaligned bytes to be written to disk.
    #endif
};