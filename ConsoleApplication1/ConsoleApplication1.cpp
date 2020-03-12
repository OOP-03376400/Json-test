
#include "jsmn.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define u0_dbg_printf printf
#define strcpy strcpy_s
#define strncpy strncpy_s
typedef struct
{
	char stationID[10];
	char date[8];
	char time[8];
	bool pump;
	int v1;
	int v2;
	int v3;
	int s1;
	int s2;
	int s3;
	char ws[8];
	int wd;

} station_t;
char strout[8];
jsmn_parser p;
jsmntok_t t[256]; /* We expect no more than 128 tokens */
char RY_VALUE[5];

static int jsoneq(const char* json, jsmntok_t* tok, const char* s)
{
	if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start
		&& strncmp(json + tok->start, s, tok->end - tok->start) == 0)
	{
		return 0;
	}
	return -1;
}

static char JSON_STRING[1024];
station_t deo_station[5];
int main()
{

	char response_string[] = "[{\"ID\": \"BP05-A\",\"D\": \"110320\", \"T\": \"10.59\",\"PUMP\": \"OFF\",\"S1\": "
		"\"001\",\"V1\": 803,\"S2\": \"038\",\"V2\": 211, \"S3\": \"051\",\"V3\": 882,\"WS\": 0,\"WD\": 8	}, "
		"{ \"ID\": \"BP05-B\", \"D\": \"110320\", \"T\": \"10.59\", \"PUMP\": \"OFF\", \"S1\": \"001\", \"V1\": 803,"
		"\"S2\": \"038\",\"V2\": 211,\"S3\": \"051\",\"V3\": 882, \"WS\": 0, \"WD\": 8}, {\"ID\": \"BP05-C\",\"D\": "
		"\"110320\", \"T\": \"10.59\", \"PUMP\": \"OFF\", \"S1\": \"001\", \"V1\": 803, \"S2\": \"038\", \"V2\": 211, "
		"\"S3\": \"051\",\"V3\": 882, \"WS\": 0, \"WD\": 8 }, {\"ID\": \"BP05-D\", \"D\": \"110320\", \"T\": \"10.59\","
		"\"PUMP\": \"OFF\", \"S1\": \"001\", \"V1\": 803, \"S2\": \"038\", \"V2\": 211, \"S3\": \"051\", \"V3\": 882, "
		"\"WS\": 0, \"WD\": 8}, {\"ID\": \"BP05-D\", \"D\": \"110320\", \"T\": \"10.59\","
		"\"PUMP\": \"OFF\", \"S1\": \"001\", \"V1\": 803, \"S2\": \"038\", \"V2\": 211, \"S3\": \"051\", \"V3\": 882, "
		"\"WS\": 0, \"WD\": 8},  {\"RY\": 128}]";
	int i = 0;
	int r = 0;
	int j = 0;
	char temp[10] =
	{ 0 };
	strcpy(JSON_STRING, response_string);

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t,
		sizeof(t) / sizeof(t[0]));
	if (r < 0)
	{
		u0_dbg_printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	if (t[0].type == JSMN_ARRAY)
	{
		for (i = 1; i < r; i++)
		{
			if (t[i].type == JSMN_OBJECT)
			{
				j++;
			}
			if (jsoneq(JSON_STRING, &t[i], "ID") == 0)
			{
				strncpy(deo_station[j - 1].stationID,
					JSON_STRING + t[i + 1].start,
					t[i + 1].end - t[i + 1].start);
				i++;
			}
			else if (jsoneq(JSON_STRING, &t[i], "D") == 0)
			{
				strncpy(deo_station[j - 1].date, JSON_STRING + t[i + 1].start,
					t[i + 1].end - t[i + 1].start);
				i++;
			}
			else if (jsoneq(JSON_STRING, &t[i], "T") == 0)
			{
				strncpy(deo_station[j - 1].time, JSON_STRING + t[i + 1].start,
					t[i + 1].end - t[i + 1].start);
				i++;
			}
			else if (jsoneq(JSON_STRING, &t[i], "PUMP") == 0)
			{
				strncpy(temp, JSON_STRING + t[i + 1].start,
					t[i + 1].end - t[i + 1].start);

				u0_dbg_printf("JSON string PUMP = %s\r\n", temp);

				if (strcmp(temp, "ON") != 0)
					deo_station[j - 1].pump = false;
				else
					deo_station[j - 1].pump = true;
				i++;
			}
			else if (jsoneq(JSON_STRING, &t[i], "S1") == 0)
			{
				strncpy(temp, JSON_STRING + t[i + 1].start,
					t[i + 1].end - t[i + 1].start);
				deo_station[j - 1].s1 = atoi(temp);
				i++;
			}
			else if (jsoneq(JSON_STRING, &t[i], "V1") == 0)
			{
				strncpy(temp, JSON_STRING + t[i + 1].start,
					t[i + 1].end - t[i + 1].start);
				deo_station[j - 1].v1 = atoi(temp);
				i++;
			}

			else if (jsoneq(JSON_STRING, &t[i], "S2") == 0)
			{
				strncpy(temp, JSON_STRING + t[i + 1].start,
					t[i + 1].end - t[i + 1].start);
				deo_station[j - 1].s2 = atoi(temp);
				i++;
			}

			else if (jsoneq(JSON_STRING, &t[i], "V2") == 0)
			{
				strncpy(temp, JSON_STRING + t[i + 1].start,
					t[i + 1].end - t[i + 1].start);
				deo_station[j - 1].v2 = atoi(temp);
				i++;
			}

			else if (jsoneq(JSON_STRING, &t[i], "S3") == 0)
			{
				strncpy(temp, JSON_STRING + t[i + 1].start,
					t[i + 1].end - t[i + 1].start);
				deo_station[j - 1].s3 = atoi(temp);

				i++;
			}
			else if (jsoneq(JSON_STRING, &t[i], "V3") == 0)
			{
				strncpy(temp, JSON_STRING + t[i + 1].start,
					t[i + 1].end - t[i + 1].start);
				deo_station[j - 1].v3 = atoi(temp);
				i++;
			}

			else if (jsoneq(JSON_STRING, &t[i], "WS") == 0)
			{
				strncpy(deo_station[j - 1].ws, JSON_STRING + t[i + 1].start,
					t[i + 1].end - t[i + 1].start);
				i++;
			}
			else if (jsoneq(JSON_STRING, &t[i], "WD") == 0)
			{
				strncpy(temp, JSON_STRING + t[i + 1].start,
					t[i + 1].end - t[i + 1].start);
				deo_station[j - 1].wd = atoi(temp);
				i++;
			}
		}
		i--;
		printf("i=%d\n", i);
		if (jsoneq(JSON_STRING, &t[i-1], "RY") == 0)
		{
			strncpy(RY_VALUE ,
				JSON_STRING + t[i].start,
				t[i ].end - t[i].start);
			i++;
			printf("RY=%s\n", RY_VALUE);
		}




	}



	for (int x = 0; x < 5; x++)
	{
		u0_dbg_printf("%d\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%d\t%s\t%s\r\n",
			x, deo_station[x].stationID,
			deo_station[x].pump == 0 ? "OFF" : "ON", deo_station[x].v1,
			deo_station[x].v2, deo_station[x].v3, deo_station[x].s1,
			deo_station[x].s2, deo_station[x].s3, deo_station[x].ws,
			deo_station[x].wd, deo_station[x].date, deo_station[x].time);
	}
}
