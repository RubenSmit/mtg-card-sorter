# Magic the Gathering Card Sorter
This project will provide a cheap and simple Magic the Gathering Card Sorter for home use. It will not be the best and fastest sorter with the largest capacity, others have way better solutions for that. But this will be simple to build  and capable of sorting your personal library.

> [!NOTE]  
> This project is a work in progress. The sorter does not work yet. There is no estimated date of completion.

## How does it work
The [Delver Lens](https://www.delverlab.com/) android app will be use to scan and catalog the cards. This app uses the camera of your phone to detect the cards and can communicate with the internet about what kind of card it just scanned. We use this communication to pick up the card and place it in the correct bin. The next card will then be scanned and we repeat the process untill all cards are sorted.  
The card loader has a capacity of about *(to be determined, probably 500)* cards but you can extend this if needed. The robot arm has a suction cup to pick up the cards without damaging them.

## Getting started
1. Download and install the [Delver Lens android app](https://www.delverlab.com/) on your phone.
2. Connect the card sorter to your home wifi network.
3. Navigate to [http://sorter.local](http://sorter.local).
4. Copy the GET url that Delver Lens will use to connect to the sorter.
5. In the Delver Lens app open the settings and scroll to HTTP get.
	1. Enable HTTP get.
	2. Set the url to the one provided.
6. Load cards into the sorter.
7. Place your phone into the holder of the sorter.
8. Press the `start` button on the sorter. The sorter will now load the first card.
9. Start scanning the cards with the app. Enshure the camera of your phone is focusse on the loaded card.

The sorter will now automatically place the scanned card in the correct bin by color. To stop sorting perfor the following steps:

1. Press the `stop` button on the sorter.
2. Stop scanning cards on your phone.

## Building your own
Work in progress...