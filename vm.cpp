#include <iostream>
#include <fstream>
#include <string> 
using namespace std;

//class to store page entry data
class page_entry{
    public:
    page_entry();
    int use_bit;
    string page;
    
};

//constructor
page_entry::page_entry(){
    use_bit=0; //determines if page has been accessed
    page=""; //page frame address
}




int main(int argc, char**argv){
    
    ifstream fin;
    ofstream fout;
    
    fin.open(argv[1]);
    if(fin.fail()){
        cout<<"file failed to open"<<endl;
        return(-1);
    }
    
    fout.open("vm-out.txt");
    if(fout.fail()){
        cout<<"file failed to open"<<endl;
        return(-1);
    }
    
    
    string address[32]; //store address patterns
    string address_pattern[32]; //need only first 5 hex patterns to distinguish addresses, position in address array corresponds to address_pattern position 
    page_entry page_table[4]; //page table with 4 page entries
    
    int address_count=0; //index counter for address array
    string current_line; //read in current line from file 
    string current_pattern; //extract first 5 hex patterns from line
    int print_address_index; //index to locate address to be saved in page table
    int clock_index=0; //clock index to go through replacement algorithm 
    
    //get first 32 lines and store them as entries
    while(address_count < 32){
        fin>>current_line;
        address[address_count]=current_line;  
        
        
    //some addresses have irregular address, not size 8
    //need to add leading 0s to become length 8
       while(current_line.length() < 8){
           current_line.insert(0,"0");
       }
       

        //extract bit patterns, ignoring offset
        for(int i=0; i<5; i++){
            address_pattern[address_count]+= current_line[i];
        }
        
        address_count++; //get next address
    }
    
    
    //all lines after, insert into page table
    while(fin>>current_line){
        
       while(current_line.length() < 8){
           current_line.insert(0,"0");
       }
        bool found_zero=false;
        //get first 5 hex values to compare with address_pattern array
        for(int i=0; i<5; i++){
            current_pattern += current_line[i];
        }
        //find the pattern that corresponds to address
        for(int i=0; i<32; i++){
            if(address_pattern[i] == current_pattern){
                print_address_index=i;//save location
            }
        }
    
        
        //first determine if the address is already in the page table 
        for(int i=0; i<4; i++){
            if(page_table[i].page == address[print_address_index]){
                page_table[i].use_bit = 1; 
                found_zero=true;
            }
        }
        
        //use clock algorithm
        while(found_zero == false){
            //if you find a zero, set page and increment clock
            if(page_table[clock_index].use_bit == 0){
                page_table[clock_index].page = address[print_address_index]; //assign page address to entry
                page_table[clock_index].use_bit = 1; //set bit
                clock_index++; //increment clock 
                
                //wrap clock index around
                if(clock_index==4){
                    clock_index=0;
                }
                found_zero=true;
            }
            
            //increment to next position and find a zero, set current page use bit to 0
            else{
                page_table[clock_index].use_bit = 0; //reset use bit
                clock_index++;//increment clock and wrap around 
                if(clock_index==4){
                    clock_index=0;
                }
            }
            
        }
        
    current_pattern = "";    
    //print results    
  
    //only one entry
    if(page_table[1].page==""){
        fout<<page_table[0].page<<endl;
    }
    
    //2 entries
    else if(page_table[2].page==""){
        fout<<page_table[0].page<<" "<<page_table[1].page<<endl;
    }
    
    //3 entries
    else if(page_table[3].page==""){
        fout<<page_table[0].page<<" "<<page_table[1].page<<" "<<page_table[2].page<<endl;
    }
    
    //4 entries
    else{
        fout<<page_table[0].page<<" "<<page_table[1].page<<" "<<page_table[2].page<<" "<<page_table[3].page<<endl; 
    }
     
        
    } //end of while loop for all lines after first 32
    
    
    fin.close();
    fout.close();
    
    return 0;
}