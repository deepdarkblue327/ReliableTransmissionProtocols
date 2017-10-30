#include "../include/simulator.h"
#include "../include/helper.h"
#include<iostream>
#include<string>
#include<vector>
using namespace std;

string buffer[1000];
int seqno[1000];
int index = 0;

struct pkt gen_pkt(string message, int seqnum) {
    struct pkt p;
    p.seqnum = seqnum;
    p.acknum = seqnum;
    for(int i = 0; i < 20; i++) {
        p.payload[i] = (char)message[i];
    }
    p.checksum = 0;
    for(int i = 0; i < 20; i++) {
        p.checksum += (int)p.payload[i];
    }
    p.checksum += p.seqnum + p.acknum;
    return p;
}

bool validate_checksum(struct pkt p) {
    int checksum = 0;
    for(int i = 0; i < 20; i++) {
        checksum += (int)p.payload[i];
    }
    checksum += p.seqnum + p.acknum;
    if(checksum == p.checksum) {
        return true;
    } else {
        return false;
    }
}

/* ******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for PA2, unidirectional data transfer
   protocols (from A to B). Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
     (although some can be lost).
**********************************************************************/

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{
    buffer[index++] = (char*)message.data;
    for(int i = 0; i < 1000; i++) {
        if(buffer[i] != "") {
            tolayer3(0,gen_pkt(buffer[i],seqno[i]));
            starttimer(0,20.0);
            break;
        }
    }
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
    if(validate_checksum(packet)) {
        cout<<"DATE"<<endl;
        for(int i = 0; i < 1000; i++) {
            if(buffer[i]!= "") {
                buffer[i] = "";
                cout<<"YEEE"<<endl;
                stoptimer(0);
            }
        }
    }

}

/* called when A's timer goes off */
void A_timerinterrupt()
{
    for(int i = 0; i < 1000; i++) {
        if(buffer[i]!= "") {
            tolayer3(0,gen_pkt(buffer[i],seqno[i]));
        }
    }

}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
    for(int i = 0; i < 1000; i++) {
        seqno[i] = i%2;
    }
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet)
{
    if(validate_checksum(packet)) {
        cout<<"NOOO"<<endl;
        tolayer3(1,packet);
        tolayer5(1,packet.payload);
    }
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{

}
