#include "../include/simulator.h"
#include<iostream>
#include<string>
#include<vector>
using namespace std;

#define BUFFER 1000
#define MSG_SIZE 20
#define INTERRUPT 12.0


struct pkt gen_pkt(string message, int seqnum) {
    struct pkt p;
    p.seqnum = seqnum;
    p.acknum = seqnum;
    for(int i = 0; i < MSG_SIZE; i++) {
        p.payload[i] = (char)message[i];
    }
    p.checksum = 0;
    for(int i = 0; i < MSG_SIZE; i++) {
        p.checksum += (int)p.payload[i];
    }
    p.checksum += p.seqnum + p.acknum;
    return p;
}

bool validate_checksum(struct pkt p) {
    int checksum = 0;
    for(int i = 0; i < MSG_SIZE; i++) {
        checksum += (int)p.payload[i];
    }
    checksum += p.seqnum + p.acknum;
    if(checksum == p.checksum) {
        return true;
    } else {
        return false;
    }
}

vector<pkt> buf;
vector<int> seqno(1000,-1);
int seqnum = -1;
int index = 0;
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
    buf.push_back(gen_pkt((char*)message.data,index%2));
    if(seqno[0] == -1) {
        tolayer3(0,buf[0]);
        seqno[0] = 0;
        starttimer(0,INTERRUPT);
    }
    index++;
    /*buffer[index++] = (char*)message.data;
    for(int i = acked; i < BUFFER; i++) {
        if(buffer[i] != "") {
            if(acks[i] != -1) {
                return;
            }
            //cout<<"starttimer"<<endl;
            starttimer(0,INTERRUPT);
            tolayer3(0,gen_pkt(buffer[i],seqno[i]));
            acks[i] = 0;
            break;
        }
    }
    */
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
    if(validate_checksum(packet)) {
        if(packet.acknum == buf[0].acknum) {
            buf.erase(buf.begin());
            seqno.erase(seqno.begin());
            stoptimer(0);
        }
    }
}

/* called when A's timer goes off */
void A_timerinterrupt()
{
    starttimer(0,INTERRUPT);
    tolayer3(0,buf[0]);
}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{

}

/* Note that with simplex transfer from a-to-B, there is no B_output() */
int b_ack = 0;
/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet)
{
    if(validate_checksum(packet)) {

        if(b_ack%2 == packet.acknum) {
            tolayer5(1,packet.payload);

            tolayer3(1,packet);
            b_ack+=1;
        }
    }
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{

}
