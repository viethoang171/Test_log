/*******************************************************************************
 *				 _ _                                             _ _
                |   |                                           (_ _)
                |   |        _ _     _ _   _ _ _ _ _ _ _ _ _ _   _ _
                |   |       |   |   |   | |    _ _     _ _    | |   |
                |   |       |   |   |   | |   |   |   |   |   | |   |
                |   |       |   |   |   | |   |   |   |   |   | |   |
                |   |_ _ _  |   |_ _|   | |   |   |   |   |   | |   |
                |_ _ _ _ _| |_ _ _ _ _ _| |_ _|   |_ _|   |_ _| |_ _|
                                (C)2021 Lumi
 * Copyright (c) 2021
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File name: Example.c
 *
 * Description: This code is used for tranning Lumi IOT member. It is the code form statandard.
 *
 * Author: PhuongNP
 *
 * Last Changed By:  $Author: phuongnp $
 * Revision:         $Revision: $
 * Last Changed:     $Date: $Jun 9, 2021
 *
 * Code sample:
 ******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
#include <stdio.h>
#include <stdlib.h>
#include "typedefs.h"
#include <string.h>
/******************************************************************************/

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
#define MAX_LENGTH_FILE 5000
#define FILENAME "log.txt"
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
i8_t g_fileStr[MAX_LENGTH_FILE];
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
i32_t fileToStr(i8_p pibStr);

i32_t count(i8_p pibFileLog);

void_t task1();

void_t task2();

void_t task3();

void_t task4();

void_t task5();

u32_t pow10(u32_t i);

u32_t stringToNum(char_t byStr[MAX_LENGTH_FILE]);
void_t task6();

i32_t main()
{
    fileToStr(g_fileStr);
    task1();
    task2();
    task3();
    task4();
    task5();
    task6();
    return 0;
}

i32_t fileToStr(i8_p pibStr)
{
    i32_t dwStatus;
    FILE *fp = NULL;
    fp = fopen(FILENAME, "r");
    if (fp == NULL)
    {
        printf("File does not exist\n");
        return -1;
    }
    dwStatus = fread(pibStr, MAX_LENGTH_FILE, 1, fp);
    fclose(fp);
    fp = NULL;
    return dwStatus;
}

i32_t count(i8_p pibFileLog)
{
    i32_t dwCount = 0;
    i8_t byDirect[] = "\"cmd\":\"set\"";
    i8_p pbyCheckEnd = NULL;
    do
    {
        if (pbyCheckEnd == NULL)
            pbyCheckEnd = strstr(pibFileLog, byDirect);
        else
            pbyCheckEnd = strstr(pbyCheckEnd + 1, byDirect);
        if (pbyCheckEnd != NULL)
            dwCount++;
    } while (pbyCheckEnd != NULL);
    return dwCount;
}

u32_t pow10(u32_t i)
{
    if (i == 0)
        return 1;
    u32_t dwResult = 1;
    for (u32_t k = 1; k <= i; k++)
        dwResult *= 10;
    return dwResult;
}
u32_t stringToNum(char_t byStr[MAX_LENGTH_FILE])
{
    u32_t dwResult = 0;
    u32_t dwLengthStr = strlen(byStr);
    for (u32_t i = 0; i < dwLengthStr; i++)
    {
        u32_t dwChuSo = byStr[dwLengthStr - i - 1] - 48;
        dwResult += dwChuSo * pow10(i);
    }
    return dwResult;
}
void_t task5()
{
    u32_t dwMaxDelay = 0;
    char_t byEnd[MAX_LENGTH_FILE] = "";
    byEnd[0] = 10;
    char_p pbyCheckEnd = g_fileStr;

    char_t byMinuteSet[MAX_LENGTH_FILE] = "";
    char_t byMinuteStatus[MAX_LENGTH_FILE] = "";
    char_t bySecondSet[MAX_LENGTH_FILE] = "";
    char_t bySecondStatus[MAX_LENGTH_FILE] = "";

    char_t byReqidSet[MAX_LENGTH_FILE] = "";
    char_t byReqidStatus[MAX_LENGTH_FILE] = "";
    while (pbyCheckEnd != NULL)
    {
        char_p pbyFindMinute = strstr(pbyCheckEnd, ":");

        // xu ly Time
        char_p pbyFindSecond = strstr(pbyFindMinute + 1, ":");
        char_p pbyFindEndTime = strstr(pbyFindSecond + 1, "]");
        char_t byDelayMinute[MAX_LENGTH_FILE] = "";
        char_t byDelaySecond[MAX_LENGTH_FILE] = "";
        u32_t dwLengthMinute = pbyFindSecond - pbyFindMinute - 1;
        for (u32_t i = 0; i < dwLengthMinute; i++)
            byDelayMinute[i] = *(pbyFindMinute + i + 1);
        u32_t dwLengthSecond = pbyFindEndTime - pbyFindSecond - 1;
        for (u32_t i = 0; i < dwLengthSecond; i++)
            byDelaySecond[i] = *(pbyFindSecond + i + 1);

        // FindDirect
        char_p pbyFindDirect = strstr(pbyCheckEnd, "\"cmd\":");
        u32_t dwLengthDirect = strlen("\"cmd\":");
        char_p pbyEndDirect = strstr(pbyCheckEnd, ",");
        char_t byDirect[MAX_LENGTH_FILE] = "";
        for (u32_t i = 0; i < pbyEndDirect - pbyFindDirect - dwLengthDirect; i++)
            byDirect[i] = *(pbyFindDirect + i + dwLengthDirect);

        // FindCorrectNews
        char_p pbyFindReqid = strstr(pbyCheckEnd, "\"reqid\": \"");
        u32_t dwLengthReqid = strlen("\"reqid\": \"");
        char_p pbyFindReqidEnd = strstr(pbyFindReqid + dwLengthReqid, "\"");
        u32_t dwLengthReqidCode = pbyFindReqidEnd - pbyFindReqid - dwLengthReqid;
        if (strcmp(byDirect, "\"set\"") == 0)
        {
            byReqidSet[MAX_LENGTH_FILE] = "";
            for (u32_t i = 0; i < dwLengthReqidCode; i++)
                byReqidSet[i] = *(pbyFindReqid + i + dwLengthReqid);
        }
        else
        {
            byReqidStatus[MAX_LENGTH_FILE] = "";
            for (u32_t i = 0; i < dwLengthReqidCode; i++)
                byReqidStatus[i] = *(pbyFindReqid + i + dwLengthReqid);
            if (strcmp(byReqidSet, byReqidStatus) != 0)
            {
                pbyCheckEnd = strstr(pbyCheckEnd + 1, byEnd);
                continue;
            }
        }

        // Xu ly Direct
        if (strcmp(byDirect, "\"set\"") == 0)
        {
            byMinuteSet[MAX_LENGTH_FILE] = "";
            bySecondSet[MAX_LENGTH_FILE] = "";
            for (u32_t k = 0; k < dwLengthMinute; k++)
                byMinuteSet[k] = *(byDelayMinute + k);
            u32_t dwLengthSecondSet = 0;
            for (u32_t k = 0; k < dwLengthSecond; k++)
                if (*(byDelaySecond + k) != '.')
                    bySecondSet[dwLengthSecondSet++] = *(byDelaySecond + k);
        }
        else
        {
            byMinuteStatus[MAX_LENGTH_FILE] = "";
            bySecondStatus[MAX_LENGTH_FILE] = "";
            for (u32_t k = 0; k < dwLengthMinute; k++)
                byMinuteStatus[k] = *(byDelayMinute + k);
            u32_t dwLengthSecondStatus = 0;
            for (u32_t k = 0; k < dwLengthSecond; k++)
                if (*(byDelaySecond + k) != '.')
                    bySecondStatus[dwLengthSecondStatus++] = *(byDelaySecond + k);
            u32_t dwTimeDelay = (stringToNum(byMinuteStatus) - stringToNum(byMinuteSet)) * 60 * 1000 + (stringToNum(bySecondStatus) - stringToNum(bySecondSet));
            if (dwTimeDelay > dwMaxDelay)
                dwMaxDelay = dwTimeDelay;
        }

        pbyCheckEnd = strstr(pbyCheckEnd + 1, byEnd);
    }
    printf("Do tre lon nhat la: %d Millisecond\n", dwMaxDelay);
}

void_t task1()
{
    u32_t dwCount = count(g_fileStr);
    printf("\nSo luong ban tin la: %d", dwCount);
}

void_t task2()
{
    i8_t byAddressNWK[MAX_LENGTH_FILE] = "";
    printf("\nNhap dia chi nwk cua thiet bi: %");
    scanf("%s", byAddressNWK);
    printf("\n");
    i8_p pbyCheckEnd = g_fileStr;
    i8_t byEnd[MAX_LENGTH_FILE] = "";
    byEnd[0] = 10;
    i8_p pbyStart = NULL;
    i8_p pbyEnd = NULL;
    u32_t dwCountNews = 0;
    while (pbyCheckEnd != NULL)
    {
        i8_t byNews[MAX_LENGTH_FILE] = "";
        u32_t dwNewsIndex = 0;
        pbyStart = strstr(pbyCheckEnd, "[INFO]");
        pbyEnd = strstr(pbyCheckEnd + 1, byEnd);
        i8_p pibIndexNews = NULL;
        if (pbyStart - pbyEnd < 0)
            for (pibIndexNews = pbyStart; pibIndexNews != pbyEnd + 1; pibIndexNews++)
            {
                byNews[dwNewsIndex++] = *pibIndexNews;
            }

        i8_p pbyFindSet = strstr(byNews, "\"set\"");
        if (pbyFindSet == NULL)
        {
            pbyCheckEnd = strstr(pbyCheckEnd + 1, byEnd);
            continue;
        }
        i8_p pbyFindNwk = strstr(byNews, byAddressNWK);
        if (pbyFindNwk != NULL)
            dwCountNews++;
        pbyCheckEnd = strstr(pbyCheckEnd + 1, byEnd);
    }
    printf("\nSo ban tin gui di la: %d\n", dwCountNews);
}

void_t task3()
{
    i8_t pbyBufferString[20][MAX_LENGTH_FILE];
    i8_t pbyBufferEndPoint[20][MAX_LENGTH_FILE];
    u32_t dwNumberBufferString = 0;
    i8_t byEnd[MAX_LENGTH_FILE] = "";
    byEnd[0] = 10;
    i8_p pbyCheckEnd = g_fileStr;
    i8_p pbyStart = NULL;
    i8_p pbyEnd = NULL;
    while (pbyCheckEnd != NULL)
    {
        i8_t byNews[MAX_LENGTH_FILE] = "";
        u32_t dwNewsIndex = 0;
        pbyStart = strstr(pbyCheckEnd, "[INFO]");
        pbyEnd = strstr(pbyCheckEnd + 1, byEnd);
        i8_p pibNewsIndex = NULL;
        if (pbyStart - pbyEnd < 0)
            for (pibNewsIndex = pbyStart; pibNewsIndex != pbyEnd + 1; pibNewsIndex++)
            {
                byNews[dwNewsIndex++] = *pibNewsIndex;
            }

        i8_p pbyFindSet = strstr(byNews, "\"set\"");
        if (pbyFindSet == NULL)
        {
            pbyCheckEnd = strstr(pbyCheckEnd + 1, byEnd);
            continue;
        }
        i8_p pbyNewsCode = NULL;
        pbyNewsCode = strstr(pbyCheckEnd, "\"zwave");

        i8_p pbyNetWork = NULL;
        pbyNetWork = strstr(pbyNewsCode, "-");
        u32_t dwLengthNetWork = 0;
        i8_t byResultNetWork[MAX_LENGTH_FILE] = "";
        while (*(pbyNetWork + dwLengthNetWork) != ':')
        {
            byResultNetWork[dwLengthNetWork++] = *(pbyNetWork + dwLengthNetWork);
        }
        i8_p pbyEndPoint = NULL;
        pbyEndPoint = strstr(pbyNetWork + dwLengthNetWork, "-");
        u32_t dwLengthEndPoint = 0;
        i8_t byResultEndPoint[MAX_LENGTH_FILE] = "";
        while (*(pbyEndPoint + dwLengthEndPoint) != '"')
            byResultEndPoint[dwLengthEndPoint++] = *(pbyEndPoint + dwLengthEndPoint);
        u32_t dwCheckString = 0;
        for (u32_t i = 1; i <= dwNumberBufferString; i++)
            if (strcmp(byResultNetWork, pbyBufferString[i]) == 0)
            {
                dwCheckString = 1;
                break;
            }
        if (dwCheckString == 0)
        {
            ++dwNumberBufferString;
            for (u32_t j = 0; j < strlen(byResultEndPoint); j++)
                pbyBufferEndPoint[dwNumberBufferString][j] = byResultEndPoint[j];
            for (u32_t j = 0; j < strlen(byResultNetWork); j++)
                pbyBufferString[dwNumberBufferString][j] = byResultNetWork[j];
        }
        pbyCheckEnd = strstr(pbyCheckEnd + 1, byEnd);
    }
    for (u32_t i = 1; i <= dwNumberBufferString; i++)
        printf("thiet bi %d co dia chi la : NWK %s, ENDPOINT %s\n", i, pbyBufferString[i], pbyBufferEndPoint[i]);
}

void_t task4()
{
    u32_t dwCountErrorNews = 0;
    i8_p pbyCheckEnd = g_fileStr;
    i8_t byEnd[MAX_LENGTH_FILE] = "";
    byEnd[0] = 10;
    i8_t byReqidSet[MAX_LENGTH_FILE] = "";
    i8_t byReqidStatus[MAX_LENGTH_FILE] = "";

    while (pbyCheckEnd != NULL)
    {
        i8_p pbyEndNews = strstr(pbyCheckEnd + 1, byEnd);
        i8_p pbyFindDirect = strstr(pbyCheckEnd, "\"cmd\":");
        u32_t dwLengthDirect = strlen("\"cmd\":");
        i8_p pbyEndDirect = strstr(pbyCheckEnd, ",");
        i8_t byDirect[MAX_LENGTH_FILE] = "";
        for (u32_t i = 0; i < pbyEndDirect - pbyFindDirect - dwLengthDirect; i++)
            byDirect[i] = *(pbyFindDirect + i + dwLengthDirect);

        i8_p pbyFindReqid = strstr(pbyCheckEnd, "\"reqid\": \"");
        u32_t dwLengthReqid = strlen("\"reqid\": \"");
        i8_p pbyFindReqidEnd = strstr(pbyFindReqid + dwLengthReqid, "\"");
        u32_t dwLengthReqidCode = pbyFindReqidEnd - pbyFindReqid - dwLengthReqid;
        if (strcmp(byDirect, "\"set\"") == 0)
        {
            byReqidSet[MAX_LENGTH_FILE] = "";
            for (u32_t i = 0; i < dwLengthReqidCode; i++)
                byReqidSet[i] = *(pbyFindReqid + i + dwLengthReqid);
        }
        else
        {
            byReqidStatus[MAX_LENGTH_FILE] = "";
            for (u32_t i = 0; i < dwLengthReqidCode; i++)
                byReqidStatus[i] = *(pbyFindReqid + i + dwLengthReqid);
            if (strcmp(byReqidSet, byReqidStatus) != 0)
                dwCountErrorNews++;
        }

        pbyCheckEnd = strstr(pbyCheckEnd + 1, byEnd);
    }
    printf("\nSo ban tin loi: %d\n", dwCountErrorNews);
}

void_t task6()
{
    u32_t dwSumTimeDelay = 0;
    u32_t dwCountCorrectNews = 0;
    i8_t byEnd[MAX_LENGTH_FILE] = "";
    byEnd[0] = 10;
    i8_p pbyCheckEnd = g_fileStr;

    i8_t byMinuteSet[MAX_LENGTH_FILE] = "";
    i8_t byMinuteStatus[MAX_LENGTH_FILE] = "";
    i8_t bySecondSet[MAX_LENGTH_FILE] = "";
    i8_t bySecondStatus[MAX_LENGTH_FILE] = "";

    i8_t byReqidSet[MAX_LENGTH_FILE] = "";
    i8_t byReqidStatus[MAX_LENGTH_FILE] = "";
    while (pbyCheckEnd != NULL)
    {
        i8_p pbyFindMinute = strstr(pbyCheckEnd, ":");

        // xu ly Time
        i8_p pbyFindSecond = strstr(pbyFindMinute + 1, ":");
        i8_p pbyFindEndTime = strstr(pbyFindSecond + 1, "]");
        i8_t byDelayMinute[MAX_LENGTH_FILE] = "";
        i8_t byDelaySecond[MAX_LENGTH_FILE] = "";
        u32_t dwLengthMinute = pbyFindSecond - pbyFindMinute - 1;
        for (u32_t i = 0; i < dwLengthMinute; i++)
            byDelayMinute[i] = *(pbyFindMinute + i + 1);
        u32_t dwLengthSecond = pbyFindEndTime - pbyFindSecond - 1;
        for (u32_t i = 0; i < dwLengthSecond; i++)
            byDelaySecond[i] = *(pbyFindSecond + i + 1);

        // FindDirect
        i8_p pbyFindDirect = strstr(pbyCheckEnd, "\"cmd\":");
        u32_t dwLengthDirect = strlen("\"cmd\":");
        i8_p pbyEndDirect = strstr(pbyCheckEnd, ",");
        i8_t byDirect[MAX_LENGTH_FILE] = "";
        for (u32_t i = 0; i < pbyEndDirect - pbyFindDirect - dwLengthDirect; i++)
            byDirect[i] = *(pbyFindDirect + i + dwLengthDirect);

        // FindCorrectNews
        i8_p pbyFindReqid = strstr(pbyCheckEnd, "\"reqid\": \"");
        u32_t dwLengthReqid = strlen("\"reqid\": \"");
        i8_p pbyFindReqidEnd = strstr(pbyFindReqid + dwLengthReqid, "\"");
        u32_t dwLengthReqidCode = pbyFindReqidEnd - pbyFindReqid - dwLengthReqid;
        if (strcmp(byDirect, "\"set\"") == 0)
        {
            byReqidSet[MAX_LENGTH_FILE] = "";
            for (u32_t i = 0; i < dwLengthReqidCode; i++)
                byReqidSet[i] = *(pbyFindReqid + i + dwLengthReqid);
        }
        else
        {
            byReqidStatus[MAX_LENGTH_FILE] = "";
            for (u32_t i = 0; i < dwLengthReqidCode; i++)
                byReqidStatus[i] = *(pbyFindReqid + i + dwLengthReqid);
            if (strcmp(byReqidSet, byReqidStatus) != 0)
            {
                pbyCheckEnd = strstr(pbyCheckEnd + 1, byEnd);
                continue;
            }
            else
                dwCountCorrectNews++;
        }

        // Xu ly Direct
        if (strcmp(byDirect, "\"set\"") == 0)
        {
            byMinuteSet[MAX_LENGTH_FILE] = "";
            bySecondSet[MAX_LENGTH_FILE] = "";
            for (u32_t k = 0; k < dwLengthMinute; k++)
                byMinuteSet[k] = *(byDelayMinute + k);
            u32_t dwLengthSecondSet = 0;
            for (u32_t k = 0; k < dwLengthSecond; k++)
                if (*(byDelaySecond + k) != '.')
                    bySecondSet[dwLengthSecondSet++] = *(byDelaySecond + k);
            // printf("Set: %s, %s\n",byMinuteSet,bySecondSet);
        }
        else
        {
            byMinuteStatus[MAX_LENGTH_FILE] = "";
            bySecondStatus[MAX_LENGTH_FILE] = "";
            for (u32_t k = 0; k < dwLengthMinute; k++)
                byMinuteStatus[k] = *(byDelayMinute + k);
            u32_t dwLengthSecondStatus = 0;
            for (u32_t k = 0; k < dwLengthSecond; k++)
                if (*(byDelaySecond + k) != '.')
                    bySecondStatus[dwLengthSecondStatus++] = *(byDelaySecond + k);
            u32_t dwTimeDelay = (stringToNum(byMinuteStatus) - stringToNum(byMinuteSet)) * 60 * 1000 + (stringToNum(bySecondStatus) - stringToNum(bySecondSet));
            dwSumTimeDelay += dwTimeDelay;
        }

        pbyCheckEnd = strstr(pbyCheckEnd + 1, byEnd);
    }
    u32_t dwAveTimeDelay = dwSumTimeDelay / dwCountCorrectNews;
    printf("Do tre trung binh la: %d Millisecond\n", dwAveTimeDelay);
}