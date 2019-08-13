#include "sha256.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
using namespace std;
template <typename T>
class Blockchain {
  private:
    template <typename U>
    class Block {
      public:
        U* m_data;
        string m_prevHash;
        string m_hash;
        Block *m_next;

        Block(string prevHash, U* data, Block *next) {
          m_prevHash = prevHash;
          m_data = data;
          m_next = next;
          stringstream s;
          //hash is the hash of previous hash + data
          s << m_prevHash << m_data;
          m_hash = sha256(s.str());
        }

        //if class, T/U has to have default constructor
        //creates genesis block that has next pointing to 0
        Block() {
          m_prevHash = sha256("genesis block");
          m_next = 0;
          m_data = new U();
          calcHash(m_prevHash);
        }

        ~Block() {
          delete m_data;
          m_data = 0;
        }

        void calcHash(string prevHash) {
          stringstream s;
          m_prevHash = prevHash;
          s << m_prevHash << m_data;
          m_hash = sha256(s.str());
        }

        U getData() const {
          return *m_data;
        }

        friend ostream& operator<<(ostream& os, const Block& b) {
          os << "Data: " << b.getData() << endl << "Hash: " << b.m_hash << endl << "Previous Hash: " << b.m_prevHash;
          return os;
        }
    };

    Block<T> *m_head;
    Block<T> *m_tail;
    int m_size;

  public:
    Blockchain() {
      m_head = new Block<T>();
      m_tail = m_head;
      string lastHash = getLastHash();
      //create binary file with last hash
      ofstream outfile("lastHash.dat", ios::binary);
      outfile.write(lastHash.c_str(),lastHash.size());
      outfile.close();
      m_size=0;
      /*TESTING BINARY FILE (Reading)
      ifstream infile("lastHash.dat", ios::binary);
      string hash;
      hash.resize(m_lastHash.size());
      infile.read(&hash[0], m_lastHash.size());
      infile.close();
      cout<<"\nFile data: "<<hash<<endl;*/
    }
    
    int getSize() {
      return m_size;
    }

    string getLastHash() {
      Block<T> *current = m_head;
      string currentHash = current->m_hash;
      while (current->m_next != 0) {
        current->m_next->calcHash(currentHash);
        current = current->m_next;
      }

      return current->m_hash;
    }

    //TODO:
    /*~Blockchain(){}
    */
    //compares file's last hash to the chain's last hash
    bool verify() {
      //read from file
      string lastHash = getLastHash();
      ifstream infile("lastHash.dat", ios::binary);
      string hash;
      hash.resize(lastHash.size());
      infile.read(&hash[0], lastHash.size());
      infile.close();
      return hash == lastHash;
    }
    
    //adds block to the end, throws string exception if blockchain was compromised TODO:rework
    void addBlock(T* data) throw (string) {
      //check chain's integrity
      if(!verify()) {
        throw "Blockchain was compromised.\n";
      }
      //tail's next becomes new block with the tail's hash as previous hash, and it is pointing to null
      m_tail->m_next = new Block<T>(m_tail->m_hash, data, 0);
      //new block is the new tail
      m_tail = m_tail->m_next;
      //write new last hash to file
      string lastHash = getLastHash();
      ofstream outfile("lastHash.dat", ios::binary);
      outfile.write(lastHash.c_str(),lastHash.size());
      outfile.close();
      m_size++;
    }

    T* toArray() {
      T* arr=new T[m_size];
      if(m_head->m_next != 0) {
        Block<T> *current = m_head->m_next;
        for(int i=0;i<m_size;i++) {
          arr[i] = *(current->m_data);
          current = current->m_next;
        }
      }
      return arr;      
    }

    friend ostream& operator<<(ostream& os, const Blockchain<T>& b){
      Block<T> *current = (b.m_head)->m_next;
      while(current!= 0){
        os << *(current) << "\n";
        current = current->m_next;
      }
      return os;
    }
};
