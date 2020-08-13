#include <avr/interrupt.h>
#include "softuart.h"
#include "UART.h"
#include <string.h>
#include<stdio.h>
#include <stdbool.h>
#define DEFAULT_BUFFER_SIZE 100

typedef struct
{
	char time[9];
	char longitude[15];
	char latitude[15];


}GPS_info;

char RX_GPS_Search_buffer[DEFAULT_BUFFER_SIZE]={0};
unsigned char counter;
void Clear_REC_Buffer(void);
bool GPS_Get_time_longitude_latitude_NMEA_format(GPS_info* GPS_information );

bool GPS_change_time_zone(GPS_info* GPS_information_orginal,GPS_info* GPS_information_modified );
bool GPS_Calculating_longitude_from_NMEA(GPS_info* GPS_information_orginal,GPS_info* GPS_information_modified );
bool GPS_Calculating_latitude_from_NMEA(GPS_info* GPS_information_orginal,GPS_info* GPS_information_modified );

int main(void)
{
	char c,i=0;
	GPS_info GPS_SKM55_information_original;
	GPS_info GPS_SKM55_information_modified;

	softuart_init();
	softuart_turn_rx_on(); /* redundant - on by default */
	sei();
	UART_init();
	UART_SEND_string( "hi hello from here\r\n" );  // string "from RAM"

	while(1)
	{

		if ( softuart_kbhit() ) {
			c = softuart_getchar();
			RX_GPS_Search_buffer[counter++]=c;
			if(counter==100) 	Clear_REC_Buffer();

		}




		if(strstr(RX_GPS_Search_buffer,"GPGGA") )
		{
			UART_SEND_string( "\r\n start \r\n" );
			Clear_REC_Buffer();
		}
		if(strstr(RX_GPS_Search_buffer,"GPGSA") )
		{

			GPS_Get_time_longitude_latitude_NMEA_format(&GPS_SKM55_information_original);


			UART_SEND_string( "\r\n GMT Time zone            :  " );
			UART_SEND_string(GPS_SKM55_information_original.time);
			UART_SEND_string( "\r\n longitude in NMEA Format :  " );
			UART_SEND_string(GPS_SKM55_information_original.longitude);
			UART_SEND_string( "\r\n latitude in NMEA Format  :  " );
			UART_SEND_string(GPS_SKM55_information_original.latitude);


			GPS_change_time_zone(&GPS_SKM55_information_original,&GPS_SKM55_information_modified);
			GPS_Calculating_longitude_from_NMEA(&GPS_SKM55_information_original,&GPS_SKM55_information_modified);
			GPS_Calculating_latitude_from_NMEA(&GPS_SKM55_information_original,&GPS_SKM55_information_modified);


			UART_SEND_string( "\r\n\r\n Egypt Time zone          :  " );
			UART_SEND_string(GPS_SKM55_information_modified.time);
			UART_SEND_string( "\r\n longitude in MAPS        :  " );
			UART_SEND_string(GPS_SKM55_information_modified.longitude);
			UART_SEND_string( "\r\n latitude in MAPS         :  " );
			UART_SEND_string(GPS_SKM55_information_modified.latitude);


			UART_SEND_string( "\r\n End \r\n ");


			i=0;
			Clear_REC_Buffer();
		}





	}

	return 0; /* never reached */
}




void Clear_REC_Buffer(void)
{
	counter=0;
	memset(RX_GPS_Search_buffer,0,DEFAULT_BUFFER_SIZE);
}

bool GPS_Get_time_longitude_latitude_NMEA_format(GPS_info* GPS_information)
{
	unsigned char RX_GPS_Search_buffer_idx=0,j=0,comma_counts=0;

	memset(GPS_information->time,0,9);
	memset(GPS_information->longitude,0,15);
	memset(GPS_information->latitude,0,15);

	while(RX_GPS_Search_buffer[RX_GPS_Search_buffer_idx]!='\0')
	{

		if(RX_GPS_Search_buffer[RX_GPS_Search_buffer_idx]==',' )
		{
			comma_counts++;
			if(comma_counts==1)              //Get Time
			{
				for(j=0;j<8;j++)
				{
					GPS_information->time[j]=RX_GPS_Search_buffer[++RX_GPS_Search_buffer_idx];
					if((j==1)||(j==4))
					{
						j++;
						GPS_information->time[j]=':';
					}

				}
			}
			if(comma_counts==2)              //Get Longitude
			{
				for(j=0;j<14;j++)
				{
					RX_GPS_Search_buffer_idx++;
					if(RX_GPS_Search_buffer[RX_GPS_Search_buffer_idx]==',') break;
					GPS_information->longitude[j]=RX_GPS_Search_buffer[RX_GPS_Search_buffer_idx];
				}
			}
			if(comma_counts==3)              //Get latitude
			{
				for(j=0;j<14;j++)
				{
					RX_GPS_Search_buffer_idx++;
					if(RX_GPS_Search_buffer[RX_GPS_Search_buffer_idx]==',') break;
					GPS_information->latitude[j]=RX_GPS_Search_buffer[RX_GPS_Search_buffer_idx];
				}
			}

		}
		RX_GPS_Search_buffer_idx++;
	}



	return 0;
}




bool GPS_change_time_zone(GPS_info* GPS_information_orginal,GPS_info* GPS_information_modified )
{
	unsigned char j=0,hour;

	memset(GPS_information_modified->time,0,9);



	hour=GPS_information_orginal->time[0]-0x30;
	hour*=10;
	hour+=GPS_information_orginal->time[1]-0x30;

	if(hour==22)       hour=0;
	else if(hour==23)  hour=1;
	else	           hour+=2;

	GPS_information_modified->time[0]=(hour/10)+0x30;
	GPS_information_modified->time[1]=(hour%10)+0x30;

	for(j=2;j<8;j++)
	{
		GPS_information_modified->time[j]=hour=GPS_information_orginal->time[j];

	}


	return 1;
}


bool GPS_Calculating_longitude_from_NMEA(GPS_info* GPS_information_orginal,GPS_info* GPS_information_modified )
{

	unsigned char i=0,j=0,Variable2=0;
	char *ptr;
	char    longitude_array[10]={0},longitude_buffer[15]={0};
	unsigned long variable1=0;
	double variable3=0;

	memset(GPS_information_modified->longitude,0,15);


	for(i=0,j=0;GPS_information_orginal->longitude[i]!='\0';i++,j++)
	{
		if(GPS_information_orginal->longitude[i]=='.') i++;
		longitude_array[j]=GPS_information_orginal->longitude[i];

	}

	Variable2=longitude_array[0]-0x30;
	Variable2*=10;
	Variable2+=longitude_array[1]-0x30;


	variable1 = strtol(longitude_array+2, &ptr, 10);

	variable3= (double)variable1/(double)600000;
	variable3+=(double)Variable2;

	variable3*=1000000;

	ultoa((unsigned long)variable3,longitude_buffer,10);



	for(j=0,i=0;longitude_buffer[i]!='\0';j++,i++)
	{
		GPS_information_modified->longitude[j]=longitude_buffer[i];
		if((j==1))
		{
			j++;
			GPS_information_modified->longitude[j]='.';

		}

	}


	return 1;

}



bool GPS_Calculating_latitude_from_NMEA(GPS_info* GPS_information_orginal,GPS_info* GPS_information_modified )
{

	unsigned char i=0,j=0,Variable2=0;
	char *ptr;
	char latitude_array[10]={0},latitude_buffer[15]={0};
	unsigned long variable1=0;
	double variable3=0;
	memset(GPS_information_modified->latitude,0,15);

	for(i=0,j=0;GPS_information_orginal->latitude[i]!='\0';i++,j++)
	{

		if(GPS_information_orginal->latitude[i]=='.') i++;
		latitude_array[j]=GPS_information_orginal->latitude[i];

	}

	Variable2=latitude_array[1]-0x30;
	Variable2*=10;
	Variable2+=latitude_array[2]-0x30;


	variable1 = strtol(latitude_array+3, &ptr, 10);

	variable3= (double)variable1/(double)600000;
	variable3+=(double)Variable2;

	variable3*=1000000;

	ultoa((unsigned long)variable3,latitude_buffer,10);



	for(j=0,i=0;latitude_buffer[i]!='\0';j++,i++)
	{
		GPS_information_modified->latitude[j]=latitude_buffer[i];
		if((j==1))
		{
			j++;
			GPS_information_modified->latitude[j]='.';

		}

	}


	return 1;

}
