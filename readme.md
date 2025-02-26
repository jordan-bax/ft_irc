important sources;
1 https://chi.cs.uchicago.edu/chirc/irc_examples.html
2 https://chi.cs.uchicago.edu/chirc/irc.html#message-format
3 https://datatracker.ietf.org/doc/html/rfc2812#section-5

TODO:
User and nick functions
Fill in the emtpy <> params in messages
Make sure client is removed from channel when disconnecting

Check connection with nc -C. IOT instruction (core dumped)

LIST implementatie 

Message protocol:
Privmsg to user: <sender host> PRIVMSG <username receiver> :<message>
Privmsg to channel: <sender host> PRIVMSG <channel name> :<message>
Numeric reply: <server host> <numeric code> <username> <params> :<message>

Help messages needs to be numeric reply
185-61-55-74
:jor!~joo@185-61-55-73.185-61-55-ip.fmo-solutions.nl PRIVMSG jor : tesr

:OthNC!~OthelloNC@185-61-55-73.185-61-55-ip.fmo-solutions.nl PRIVMSG #ncTestChannel ://throw rock
:OthNC!~OthelloNC@185-61-55-73.185-61-55-ip.fmo-solutions.nl PART #ncTestChannel

:OthNC PRIVMSG OthNC :Hello me
:Angel!wings@irc.org INVITE Wiz #Dust // exemple join
:f1r4s12.codam.nl 341 rockbot OthNC rockbot #ncTestChannel// join