#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include <array>

using namespace std;



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
  
  void readHeader(){
    
    ifstream logFile;
  
    logFile.open("logs.bin", ios::in | ios::binary);
    
    if(logFile.is_open()){
      logFile.read((char*) this, sizeof(*this));
      //cout<<numRecords<<endl;
    }
    else
      cout<<"Error: File not opened.\n";
    
    logFile.close();
    
  }
  
  void writeHeader(){
    
    fstream logFile;
  
    logFile.open("logs.bin", ios::in | ios::out | ios::binary);
    
    if(logFile.is_open()){
      logFile.write((char*) this, sizeof(*this));
    }
    
    else
      cout<<"Error: File not opened.\n";

    logFile.close();
    
  }
  
  void checkInit(){
    
    if(!this->initialized){
      cout<<"Initializing.\n";
      this->initialize();
    }
  
  }
};

class TimeRecord{
  
  public:
  
  
  void writeLog(bool update){
    
    fstream logFile;
    
    logFile.open("logs.bin", ios::in | ios::out | ios::binary);
    
    if(logFile.is_open()){
      
      if(update)
        logFile.seekp(-(sizeof(*this)), ios::end);
      else
        logFile.seekp(0, ios::end);
      
      logFile.write((char*) this, sizeof(*this));
    }
    else
      cout<<"Error: File not opened.\n";
    
    logFile.close();
    
  }
  
  
  void readLog(){
  
    ifstream logFile;
    
    logFile.open("logs.bin", ios::in | ios::binary);
    
    if(logFile.is_open()){
      logFile.seekg(-(sizeof(*this)), ios::end);
        
      logFile.read((char*) this, sizeof(*this));
      
      //cout<<this->getStart()<<endl;
      
    }
    
    else
      cout<<"Error: File not opened.\n";
    
    logFile.close();
    
  }
  
  

  void setStart(){  start = time(0); }
  void setEnd(){  end = time(0); duration = end - start;  }
  
  time_t getStart(){ return this->start; }
  time_t getEnd(){ return this->end; }
  int getDuration(){ return this->duration; }
  
  
  private:
    
  time_t start;
  time_t end;
  int duration;

  
};

int checkInput(string input, bool * valid ){
  
  stringstream convert(input);
  int result;
  
  if( !(convert >> result) )
    result = 0;
  
  //cout<<result<<endl;
  
  if(result == 1 || result == 2 || result == 3 || result == 4){
    *valid = true;
    return result;
  }
  else{
    cout<<"\nInvalid Input.  Try again.\n";
    return -1;
  }

}


void listRecords(FileHeader header){
  
  TimeRecord record;
  
  struct tm * start;
  struct tm * end;
  time_t temp;
  
  int i = 0;

  ifstream logFile;
  
  logFile.open("logs.bin", ios::in | ios::binary);
  
  while(i < header.numRecords){
    logFile.seekg(sizeof(header) + (i * sizeof(record)));
    logFile.read((char*) &record, sizeof(record));
    
    temp = record.getStart();
    start = localtime(&temp);
    
    
    cout<<"\n\n\n=========== Record "<<++i<<" ==========="<<endl<<endl<<"Start:\t"<<asctime(start)<<endl;
    
    temp = record.getEnd();
    end = localtime(&temp);
    
    cout<<"End:\t"<<asctime(end)<<endl<<"Duration:     ";
    
    int hours = record.getDuration()/3600;
    int minutes = (record.getDuration()%3600)/60;
    
    cout<<hours<<" hours, "<<minutes<<" minutes\n";
   
  }
  
}

int parseArgs(string argument, bool * valid){
  
  //cout<<argument<<endl;
  
  if(argument == "start"){
    
    *valid = true;
    return 1;
  }
  else if(argument == "end"){
    
    *valid = true;
    return 2;
  }
  else if(argument == "list"){
    
    *valid = true;
    return 3;
  }
  else{
    *valid = false;
    cout<<"Invalid argument, opening menu.\n";
    return -1;
  }
  
}


int main(int argc, char *argv[]){
  
  string choice;
  int convChoice;
  FileHeader header;
  bool valid = false;
  
  header.readHeader();
  //cout<<"Num records: "<<header.numRecords<<endl;
  
  header.checkInit();
  
  if(argc > 1)
    convChoice = parseArgs((string) argv[1], &valid);
  
  while(!valid){
    cout<<"1. Start\n2. End\n3. List\n4. Quit\n\nEntry: ";
    getline(cin, choice);
    convChoice = checkInput(choice, &valid);
  }
  
  
  switch(convChoice){
    
    case 1:{
      
      if(header.openRecord){
        cout<<"Please close current record.\n";
        break;
      }
      else{
        TimeRecord * record = new TimeRecord;
        header.openRecord = true;
        
        //cout<<"Read records.\n";
        record->setStart();
        (header.numRecords)++;
        header.writeHeader();
        
        record->writeLog(false);     
        
        
        delete record;
      }
    }break;
    case 2:{
      
      if(!header.openRecord){
        cout<<"Please open a record.\n";
      }
      else{
        TimeRecord * record = new TimeRecord;
        header.openRecord = false;
        
        record->readLog();
        //cout<<record->getStart()<<endl<<record->getEnd()<<endl;
        record->setEnd();
        header.writeHeader();
        //cout<<record->getEnd()<<endl;
        
        record->writeLog(true);
        
        delete record;
      }
      
    }break;
    case 3:
      listRecords(header);
      break;
    case 4:
      break;
    
  }
  
  //header.readHeader();
  
  
  //cout<<"Num records: "<<header.numRecords<<endl<<endl<<endl;
  
  
  
  
  
  return 0;
}