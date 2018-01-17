#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include <array>

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
  
  TimeRecord(){
    start = -1;
    end = -1;
    duration = -1;
    dayOfWeek = 0;
    year = 1900;
    month = 0;
  }
  
  
  void writeLog(bool update){
    
    fstream logFile;
    
    logFile.open("logs.bin", ios::in | ios::out | ios::binary);
    
    if(logFile.is_open()){
      
      if(update)
        logFile.seekp(-32, ios::end);
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
      logFile.seekg(-32, ios::end);
        
      logFile.read((char*) this, sizeof(*this));
      
      cout<<this->getStart()<<endl;
      
    }
    
    else
      cout<<"Error: File not opened.\n";
    
    logFile.close();
    
  }
  
  

  void setStart(int sTime){this->start = sTime;}
  void setEnd(int eTime){
    this->end = eTime;
    this->duration = this->end - this->start;
  }
  
  int getStart(){ return this->start; }
  int getEnd(){ return this->end; }
  int getDuration(){ return this->duration; }
  
  
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


void listRecords(FileHeader header){
  
  TimeRecord record;
  
  int i = 0;

  ifstream logFile;
  
  logFile.open("logs.bin", ios::in | ios::binary);
  
  while(i < header.numRecords){
    logFile.seekg(sizeof(header) + (i * sizeof(record)));
    logFile.read((char*) &record, sizeof(record));
    
    cout<<"Record "<<++i<<endl<<"Start: "<<record.getStart()<<endl<<"End: "<<record.getEnd()<<endl<<"Duration: "<<record.getDuration()<<endl<<endl;
   
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
  
  cout<<"Num records: "<<header.numRecords<<endl;
  
  while(!valid){
    cout<<"1. Start\n2. End\n3. Quit\n\nEntry: ";
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
        cout<<"size: "<<sizeof(*record)<<endl;
        //cout<<"Read records.\n";
        record->setStart((int) time(0));
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
        cout<<record->getStart()<<endl<<record->getEnd()<<endl;
        record->setEnd((int) time(0));
        header.writeHeader();
        cout<<record->getEnd()<<endl;
        
        record->writeLog(true);
        
        delete record;
      }
      
    }break;
    case 3:
      break;
    
  }
  
  //header.readHeader();
  
  
  cout<<"Num records: "<<header.numRecords<<endl<<endl<<endl;
  
  listRecords(header);
  
  
  
  return 0;
}