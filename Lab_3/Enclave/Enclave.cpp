#include "Enclave_t.h"
#include <memory.h>
#include <string.h>
#include "sgx_trts.h"

const char* PersonalData[] = {
    "Karen Parker - +1-696-475-7703x668 - 72432 Annette Groves Apt. 611, Brooksfurt, New York, 47772",
    "Brad Marshall - 311-681-9022x080 - 684 Cole Estates, Port Sara, Florida, 55396",
    "Vincent Lambert - (780)319-9314x5836 - 0875 Andrew Falls Apt. 307, Masonshire, Illinois, 50568",
    "Donald Gilbert - +1-611-275-7265x53143 - 222 Underwood Court, North Jason, Nevada, 31470",
    "Susan Harris - 602.462.1020x818 - 35872 Tanner Lodge Suite 158, Riveramouth, Nevada, 12550",
    "Kyle Alvarez - 311.793.8562x87268 - 4980 Janice Port Apt. 495, Parksfort, New Mexico, 18647",
    "Matthew Thomas - 630.686.9925x187 - 538 Parker Extensions Suite 657, Smithville, Arkansas, 20758",
    "Linda Sandoval - +1-841-711-5256 - 4960 Thomas Plain Apt. 144, Katherineland, Virginia, 24453",
    "Roy Sutton - 9549926005 - 673 Payne Radial, North Masonhaven, Vermont, 53324",
    "Cindy Bryant - (817)734-2884 - 421 Nathan Bypass Suite 794, Perezstad, North Carolina, 86356",
    "Richard Marquez - 346-240-4225x2912 - 0702 Jose Ranch Suite 496, Dakotaville, Delaware, 99580",
    "Christopher Pena - 9646378183 - 62434 Virginia Lakes Apt. 703, New Andrew, Maine, 77013",
    "Andrew Robbins - 813-259-1601 - 032 Emily Pine Apt. 840, West Lisaview, Georgia, 80358",
    "Alicia Thomas - 517.504.6583 - 2922 Zimmerman Wells Apt. 865, Davidmouth, Oregon, 02450",
    "Kim Baker - 827-989-0931 - 909 April Crossing, North Larrymouth, Ohio, 80863",
    "Jodi Bray - +1-730-208-2265x455 - 3520 Collins Gateway, Cruzshire, Delaware, 29504",
    "Christian Fritz - (511)914-2871 - 9978 Atkins Path, New Mark, Connecticut, 74142",
    "Dawn Fletcher - +1-779-933-1369x06869 - 70056 Susan Port, Amandachester, South Dakota, 05172",
    "Christian Harris - 776-609-3942 - 234 Zimmerman Stream, Johnshire, Arizona, 36344",
    "Alyssa Shaw - +1-948-443-8157x0214 - 076 Bell Glen Suite 605, Amandastad, Wisconsin, 62223"
};


void getPD(char* outbuffer, const size_t len, const int i) {
    size_t array_size = sizeof(PersonalData) / sizeof(char*);
    if (i < 0 || i >= array_size) {
        memset(outbuffer, 0, len);
        return;
    }
    else {
        memcpy_s(outbuffer, len, PersonalData[i], strlen(PersonalData[i]));
        return;
    }
}
