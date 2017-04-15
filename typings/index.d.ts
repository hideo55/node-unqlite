declare module 'unqlite' {

  export const enum OpenMode {
    OPEN_READONLY = 0x00000001,
    OPEN_READWRITE = 0x00000002,
    OPEN_CREATE = 0x00000004,
    OPEN_EXCLUSIVE = 0x00000008,
    OPEN_TEMP_DB = 0x00000010,
    OPEN_NOMUTEX = 0x00000020,
    OPEN_OMIT_JOURNALING = 0x00000040,
    OPEN_IN_MEMORY = 0x00000080,
    OPEN_MMAP = 0x00000100
  }

  export class Database {
    constructor(filename: string);
    open(openMode: OpenMode, callback: (err: any) => void);
    close(callback: (err: any) => void);
    store(key: string, value: string, callback: (err: any, key: string, value: string) => void);
    append(key: string, value: string, callback: (err: any, key: string, value: string) => void);
    fetch(key: string, callback: (err: any, key: string, value: string) => void);
    delete(key: string, callback: (err: any, key: string) => void);
  }

}
