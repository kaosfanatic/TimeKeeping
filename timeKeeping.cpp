#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;


string weekday[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

class FileHeader{
  
  public:
  
  bool openRecord;
  bool initialized;
  int numRecords;
  
  void initialize(){
    openRecord = false;
    initialized = true;
    numRecords = 0;
  }
};

class TimeRecord{
  
  public:
  
  TimeRecord(){
    start = -1;
    end = -1;
    duration = -1;
    dayOfWeek = 0;
    year = 1900;
    month = 0;
  }
  
  
  void setStart(int sTime){start = sTime;}
  void setEnd(int eTime){
    end = eTime;
    duration = end - start;
  }
  int getStart(){ return start; }
  int getEnd(){ return end; }
  int getDuration(){ return duration; }
  
  
  private:
    
  long int start;
  long int end;
  int duration;
  unsigned short int dayOfWeek;
  unsigned short int year;
  unsigned short int month;
  

  
};

int checkInput(string input, bool * valid ){
  
  stringstream convert(input);
  int result;
  
  if( !(convert >> result) )
    result = 0;
  
  //cout<<result<<endl;
  
  if(result == 1 || result == 2 || result == 3){
    *valid = true;
    return result;
  }
  else{
    cout<<"\nInvalid Input.  Try again.\n";
    return -1;
  }

}

void writeLog(TimeRecord record, bool overwrite, FileHeader * header){
  
  ofstream logFile;
  
  logFile.open("logs.bin", ios::out | ios::binary);
  
  
  if(logFile.is_open()){
    if(overwrite){
      cout<<"Overwriting..\n";
      logFile.seekp(sizeof(*header) + ((header->numRecords-1) * sizeof(record)));
      logFile.write((char*) &record, sizeof(record));
      header->openRecord = false;
    }
    else{
      logFile.seekp(sizeof(*header) + (header->numRecords) * sizeof(record));
      cout<<logFile.tellp()<<endl;
      header->numRecords++;
      header->openRecord = true;
      logFile.write((char*) &record, sizeof(record));
    }
  }
  else
    cout<<"Error: File not opened.\n";
  
  cout<<"Pos after writing log: "<<logFile.tellp()<<endl;
  
  logFile.close();
}

void readLog(TimeRecord * record, bool latest, FileHeader * header){
  
  ifstream logFile;
  
  logFile.open("logs.bin", ios::in | ios::binary);
  
  if(logFile.is_open()){
    
    if(latest){
      logFile.seekg(sizeof(*header) + ((header->numRecords-1) * sizeof(*record)));
      cout<<(int) logFile.tellg()<<endl;
    }
    //cout<<(int) sizeof(*record)<<" "<<(int) logFile.tellg()<<" "<<(int) sizeof(*header)<<endl;
      
    logFile.read((char*) record, sizeof(*record));
    
    cout<<record->getStart()<<endl;
        
    
  }
  else
    cout<<"Error: File not opened.\n";
  
  logFile.close();
  
}

void readHeader(FileHeader * header){
  
  ifstream logFile;
  
  logFile.open("logs.bin", ios::in | ios::binary);
  
  if(logFile.is_open()){
    logFile.read((char*) header, sizeof(*header));
    
  }
  else
    cout<<"Error: File not opened.\n";
  
  logFile.close();
  
}

void writeHeader(FileHeader header){
  
  ofstream logFile;
  
  logFile.open("logs.bin", ios::out | ios::binary);
  
  if(logFile.is_open()){
    //logFile.seekp(0, ios::beg);
    logFile.write((char*) &header, sizeof(header));
    cout<<"Pos after writing header: "<<logFile.tellp()<<endl;
  }
  
  else
    cout<<"Error: File not opened.\n";
  
  logFile.seekp(0, ios::end);
  cout<<logFile.tellp()<<endl;
  logFile.close();
  
}

void updateLog(TimeRecord * record, FileHeader * header){
  
  readHeader(header);
  
  if(!header->openRecord){
    cout<<"Inactive.\n";
    return;
  }
  
  readLog(record, true, header);
  
    
  record->setEnd((int) time(0));
  
  
  writeLog(*record, true, header);
  writeHeader(*header);
}

void checkInit(FileHeader * header){

  readHeader(header);
  
  if(!header->initialized){
    cout<<"Initializing.\n";
    header->initialize();
    writeHeader(*header);
  }
  
}


int main(int argc, char *argv[]){
  
  string choice;
  int convChoice;
  TimeRecord * record = new TimeRecord;
  FileHeader * header = new FileHeader;
  bool valid = false;
  
  checkInit(header);
  
  
  while(!valid){
    cout<<"1. Start\n2. End\n3. Quit\n\nEntry: ";
    getline(cin, choice);
    convChoice = checkInput(choice, &valid);
  }
  
  switch(convChoice){
    
    case 1:
      record->setStart((int) time(0));
      cout<<(int) time(0)<<endl;
      writeLog(*record, false, header);
      writeHeader(*header);
      break;
    case 2:
      updateLog(record, header);
      cout<<"Duration: "<<record->getDuration()<<" seconds.\n";
      break;
    case 3:
      break;
    
  }
  
  readLog(record, true, header);
  cout<<"start: "<<record->getStart()<<endl<<"end: "<<record->getEnd()<<endl;
  cout<<"Number of Records: "<<header->numRecords<<endl;
  
  
  delete header;
  delete record;
  
  return 0;
}