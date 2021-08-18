# Outputs:
![Screenshot 2021-08-18 213110](https://user-images.githubusercontent.com/57719752/129960556-4aaea41f-7f5a-495e-b895-1b64c4de1787.jpg)

# Description

### Requirements:

We need to implement a file system with the following structure

- A Partition table with the number of files and the starting position of the first file (This takes the position 0 in the EEPROM)
- A header for each file containing the following
  - File name in the 8.3 format (8 characters for name, 3 for extension), shorter file
names are padded with spaces
  - File size in bytes
  - Pointer to the next file
  - Any flags needed
- Our file system will support
  - Bool checkIfExist (fileName)
  - Byte [] readFile (fileName)
  - Void writeFile(fileName, byte[] data)
  - Void deleteFile(fileName)
  - Unsigned short getFileLocationInMemory(fileName)
  - Unsigned Short getUsedMemory()
- Our file system should support wear levelling such that, if it deletes a file, it will only mark it as deleted and will not use that space unless absolutely necessary

### Test Script

To test your file system you will do the following operations

1. Create a flash stored array of 700 bytes
2. Write this array to a file
3. Check if file exists
4. Read the file and compare the contents to the original array
5. Delete the file
6. CheckIfFileExists
7. Write the contents of a 100 character string stored in the flash to two new files
8. Print the location of the two new files
9. Create another file with 500 bytes of data
10. Print the location of the two new files
11. Delete the 3 files
12. Print the usedMemory

Please add Serial.print logging statements to your code. After finishing add a while(1) to halt the
controller
