#include "CsvFileReader.h"

CsvFileReader::CsvFileReader():FileReader(){}

CsvFileReader::~CsvFileReader(){

}

FileReader::Error CsvFileReader::readFromFile(const QString &fileName){
    return FileReader::EmptyFile;
}
