#include "../include/simulator.h"
#include "../include/helper.h"
#include<iostream>
#include<string>
#include<vector>
using namespace std;

string buffer[1000];
int seqno[1000];
int index = 0;
int acks[1000];

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
            if(acks[i] != -1) {
                return;
            }
            //cout<<"starttimer"<<endl;
            starttimer(0,20.0);
            tolayer3(0,gen_pkt(buffer[i],seqno[i]));
            acks[i] = 0;
            break;
        }

    }
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
    if(validate_checksum(packet)) {
        for(int i = 0; i < 1000; i++) {
            string a = packet.payload;
            if(buffer[i]!= "") {
                //cout<<"stoptimer"<<endl;
                if(acks[i] == 0) {
                    stoptimer(0);
                    buffer[i] = "";
                    acks[i] = 1;
                    break;
                }
            }
        }
    }
}

/* called when A's timer goes off */
void A_timerinterrupt()
{   cout<<"Timer Interrupt\n";
    for(int i = 0; i < 1000; i++) {
        if(buffer[i]!= "") {
            starttimer(0,20.0);
            tolayer3(0,gen_pkt(buffer[i],seqno[i]));
            break;
        }
    }
}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
    for(int i = 0; i < 1000; i++) {
        seqno[i] = i%2;
        acks[i] = -1;
    }
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */
int b_ack = 0;
/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet)
{
    if(validate_checksum(packet)) {

        if(b_ack%2 == packet.acknum) {
            tolayer5(1,packet.payload);
            b_ack+=1;
        }
        tolayer3(1,packet);
    }
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{

}
