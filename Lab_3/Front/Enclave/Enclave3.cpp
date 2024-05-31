#include "Enclave3_t.h"

#include "sgx_trts.h"
#include "string.h"
//#include "memory.h"

const char* PersonalData[] = {
    "Michael Scott - michael.scott@email.com - +1-420-123-4567x890 - 123 Office Street, Scrantontown, Pennsylvania, 18503",
    "Pam Beesly - pam.beesly@workplace.com - (987)654-3210 - 456 Art Lane, Dunderaland, New Jersey, 08061",
    "Jim Halpert - jim.halpert@prank.com - 321.789.6543 - 789 Prank Avenue Apt. 101, West Shutdown, Maryland, 20785",
    "Dwight Schrute - d.schrute@beets.com - +1-555-214-7890x3456 - 112 Beet Road Suite 201, Schrutefarms, Ohio, 44512",
    "Stanley Hudson - stanley.hudson@me.com - 800-555-1212 - 44 Repetition Drive Apt. 3B, Sleepytown, Virginia, 22630",
    "Phyllis Vance - phyllis.vance@sales.com - +1-314-555-9876 - 111 Selling Street Suite 500, Vanceville, Missouri, 63101",
    "Andy Bernard - andy.bernard@yippee.com - (302)123-4567 - 567 Nard Dog Plaza, Islandville, Delaware, 19904",
    "Angela Martin - angela.martin@cats.com - 555.987.6543x123 - 890 Cat Lane Suite 456, Kittenborough, California, 94016",
    "Kevin Malone - kevin.malone@numbers.com - 6099912233 - 245 Account Avenue Apt. 789, Dunderville, New York, 10001",
    "Oscar Martinez - oscar.martinez@finance.com - 720-555-5432 - 398 Finance Boulevard, Moneytown, Colorado, 80219",
    "Meredith Palmer - meredith.palmer@party.com - 303.555.2323x678 - 568 Party Street, Palmsville, Oregon, 97006",
    "Creed Bratton - creed.bratton@mystery.com - +1-888-555-9090 - 777 Unclear Road Apt. 303, Brattonville, Florida, 33101",
    "Toby Flenderson - toby.flenderson@hr.com - (202)999-8888 - 159 HR Avenue Apt. 575, Bluesborotown, Texas, 75201",
    "Ryan Howard - ryan.howard@temp.com - 411.555.4141 - 990 Temp Lane, Howardville, Illinois, 60601",
    "Kelly Kapoor - kelly.kapoor@social.com - 7145553234x567 - 123 Social Street Apt. 606, Bollywood, Nevada, 89101",
    "Jan Levinson - jan.levinson@corporate.com - 415-555-6789 - 888 Corporate Loop Suite 999, Corporateville, Georgia, 30601",
    "Holly Flax - holly.flax@hrsolutions.com - +1-212-555-9898x3210 - 101 HR Path Suite 777, Realisticcity, Nevada, 89501",
    "David Wallace - david.wallace@dundermifflin.com - (646)555-4444 - 452 Executive Road, Bigcity, New York, 10005",
    "Jo Bennett - jo.bennett@southernsabre.com - 305-555-1212x1313 - 789 Southern Boulevard, Sabretown, Florida, 33130",
    "Nellie Bertram - nellie.bertram@british.com - 707.555.6767 - 150 British Street Suite 102, Brittown, Illinois, 62704"
};


void getPD(char* outbuffer, size_t len, int i) {
    size_t array_size = sizeof(PersonalData) / sizeof(char*);
    if (i < 0 || i >= array_size) {
        memset(outbuffer, 0, len);
        return;
    }
    else {
        memcpy(outbuffer, PersonalData[i], len);
        return;
    }
}