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
    
    ofstream logFile;
  
    logFile.open("logs.bin", ios::out | ios::binary);
    
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
  
  
  void writeLog(fstream logFile, streampos position){
    
    
    if(logFile.is_open()){
      logFile.seekp(0, ios::end);
      logFile.write((char*) this, sizeof(*this));
    }
    else
      cout<<"Error: File not opened.\n";
    
    
  }
  
  
  void readLog(int position){
  
    ifstream logFile;
    
    logFile.open("logs.bin", ios::in | ios::binary);
    
    if(logFile.is_open()){
      logFile.seekg(position);
        
      logFile.read((char*) this, sizeof(*this));
      
    }
    
    else
      cout<<"Error: File not opened.\n";
    
    logFile.close();
    
  }
  
  void updateLog(){
    
    ofstream logFile;
    
    logFile.open("logs.bin", ios::out | ios::binary);
    
    if(logFile.is_open()){
      
      logFile.seekp(sizeof(*this), ios::end);
      
      logFile.write((char*) this, sizeof(*this));
      
    }
    
    else
      cout<<"Error: File not opened.\n";
    
    logFile.close();
    
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
      
      TimeRecord records[header.numRecords + 1];
      header.openRecord = true;
      
      for(int i = 0; i<header.numRecords; i++){
        records[i].readLog(sizeof(header) + (i * sizeof(records[i])));
      }
      
      //cout<<"Read records.\n";
      records[header.numRecords].setStart(time(0));
      (header.numRecords)++;
      header.writeHeader();
      
      for(int i = 0; i<header.numRecords; i++)
        records[i].writeLog();
      
      
      header.readHeader();
      
      cout<<"Num records: "<<header.numRecords<<endl;
      
      //cout<<"Wrote records.\n";
      //delete records;
    
    }break;
    case 2:{
      TimeRecord records[header.numRecords];
      header.openRecord = false;
      
      for(int i = 0; i<header.numRecords; i++){
        records[i].readLog(sizeof(header) + i * sizeof(records[i]));
      }
     
      records[header.numRecords].setEnd(time(0));
      header.writeHeader();
      
      for(int i = 0; i<header.numRecords; i++)
        records[i].writeLog();
      
      
    }break;
    case 3:
      break;
    
  }
  
  header.readHeader();
  
  
  cout<<"Num records: "<<header.numRecords<<endl;
  
  
  
  return 0;
}