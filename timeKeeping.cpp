#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;


string weekday[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

class TimeRecord{
  
  public:
  
  TimeRecord(){
    start = -1;
    end = -1;
    duration = -1;
    dayOfWeek = 0;
    year = 1900;
    month = 0;
    active = false;
  }
  
  void toggleActive() {active = !active;};
  void setStart(int sTime){start = sTime;}
  void setEnd(int eTime){
    end = eTime;
    duration = end - start;
  }
  int getStart(){ return start; }
  int getEnd(){ return end; }
  int getDuration(){ return duration; }
  bool activated(){ return active; };
  
  
  private:
  
  bool active;
  
  int start;
  int end;
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

void writeLog(TimeRecord record, bool overwrite = false){
  
  ofstream logFile;
  
  logFile.open("logs.bin", ios::out | ios::ate | ios::binary);
  
  if(logFile.is_open()){
    if(overwrite){
      cout<<"Overwriting..\n";
      logFile.seekp(sizeof(record), ios::end);
    }
    
    logFile.write((char*) &record, sizeof(record));
  }
  else
    cout<<"Error: File not opened.\n";
  
  logFile.close();
}

void readLog(TimeRecord * record){
  
  ifstream logFile;
  
  logFile.open("logs.bin", ios::in | ios::binary | ios::ate);
  
  if(logFile.is_open()){
    
    logFile.read((char*) &(*record), sizeof(*record));
    
    logFile.seekg(sizeof(*record), ios::end);
    
    logFile.read((char*) &(*record), sizeof(*record));
    
  }
  else
    cout<<"Error: File not opened.\n";
  
  logFile.close();
}

void updateLog(TimeRecord * record){
  
    
  readLog(record);
  
  if(!record->activated()){
    cout<<"Inactive.\n";
    return;
  }
  
    
  record->setEnd((int) time(0));
  record->toggleActive();
  
  /*cout<<record->getStart()<<endl<<endl;
  cout<<record->getEnd()<<endl<<endl;
  cout<<record->getDuration()<<endl<<endl;*/
    
  writeLog(*record, true);
}


int main(int argc, char *argv[]){
  
  string choice;
  int convChoice;
  TimeRecord *record = new TimeRecord();
  record->setStart((int) time(0));
  bool valid = false;
  
  
  while(!valid){
    cout<<"1. Start\n2. End\n3. Quit\n\nEntry: ";
    getline(cin, choice);
    convChoice = checkInput(choice, &valid);
  }
  
  switch(convChoice){
    
    case 1:
      record->setStart((int) time(0));
      record->toggleActive();
      writeLog(*record);
      break;
    case 2:
      updateLog(record);
      break;
    case 3:
      delete record;
      break;
    
    
  }
  
  readLog(record);
  
  cout<<"Duration: "<<record->getDuration()<<endl;
  
  
  
  return 0;
}