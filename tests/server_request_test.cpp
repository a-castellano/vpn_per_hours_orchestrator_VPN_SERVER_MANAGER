#define BOOST_TEST_MODULE ServerRequestTest
#include <boost/test/unit_test.hpp>
#include <ServerRequest.h>
#include <VPNLock.h>

VPNLock memoryLock;

int add( int i, int j ) { return i+j+1; }

BOOST_AUTO_TEST_CASE( server_request_test )
{
	ServerRequest test00( "" );
	BOOST_CHECK_EQUAL( test00.isCorrect(), false );
	BOOST_CHECK_EQUAL( test00.getCommand(), "" );
	BOOST_CHECK_EQUAL( test00.getToken(), "" );

	ServerRequest test01( "/" );
        BOOST_CHECK_EQUAL( test01.isCorrect(), false );
        BOOST_CHECK_EQUAL( test01.getCommand(), "" );
        BOOST_CHECK_EQUAL( test01.getToken(), "" );

	ServerRequest test02( "//" );
        BOOST_CHECK_EQUAL( test02.isCorrect(), false );
        BOOST_CHECK_EQUAL( test02.getCommand(), "" );
        BOOST_CHECK_EQUAL( test02.getToken(), "" );

	ServerRequest test03( "wrong" );
        BOOST_CHECK_EQUAL( test03.isCorrect(), false );
        BOOST_CHECK_EQUAL( test03.getCommand(), "" );
        BOOST_CHECK_EQUAL( test03.getToken(), "" );

        ServerRequest test04( "wrong/" );
        BOOST_CHECK_EQUAL( test04.isCorrect(), false );
        BOOST_CHECK_EQUAL( test04.getCommand(), "" );
        BOOST_CHECK_EQUAL( test04.getToken(), "" );


        ServerRequest test05( "wrong/" );
        BOOST_CHECK_EQUAL( test05.isCorrect(), false );
        BOOST_CHECK_EQUAL( test05.getCommand(), "" );
        BOOST_CHECK_EQUAL( test05.getToken(), "" );


        ServerRequest test06( "wrong/morewrong" );
        BOOST_CHECK_EQUAL( test06.isCorrect(), false );
        BOOST_CHECK_EQUAL( test06.getCommand(), "" );
        BOOST_CHECK_EQUAL( test06.getToken(), "" );


        ServerRequest test07( "create/comethingwrong" );
        BOOST_CHECK_EQUAL( test07.isCorrect(), false );
	BOOST_CHECK_EQUAL( test07.getCommand(), "" );
        BOOST_CHECK_EQUAL( test07.getToken(), "" );


        ServerRequest test08( "destroy/comethingwrong" );
        BOOST_CHECK_EQUAL( test08.isCorrect(), false );
        BOOST_CHECK_EQUAL( test08.getCommand(), "" );
        BOOST_CHECK_EQUAL( test08.getToken(), "" );

	ServerRequest test09( "create/Dhx2YSuqX88JH2B3Vnp5DqZSuoU27D80NPhHKQP2m81Ut9t6G0H9g8NI8Q7" );
	BOOST_CHECK_EQUAL( test09.isCorrect(), false );
        BOOST_CHECK_EQUAL( test09.getCommand(), "" );
        BOOST_CHECK_EQUAL( test09.getToken(), "" );

        ServerRequest test10( "create/Dhx2YSuqX88JH2B3Vnp5DqZSuoU27D80NPhHKQP2m81Ut9t6G0H9g8NI8Q7V4" );
        BOOST_CHECK_EQUAL( test10.isCorrect(), false );
        BOOST_CHECK_EQUAL( test10.getCommand(), "" );
        BOOST_CHECK_EQUAL( test10.getToken(), "" );


        ServerRequest test11( "destroy/Dhx2YSuqX88JH2B3Vnp5DqZSuoU27D80NPhHKQP2m81Ut9t6G0H9g8NI8Q7" );
        BOOST_CHECK_EQUAL( test11.isCorrect(), false );
        BOOST_CHECK_EQUAL( test11.getCommand(), "" );
        BOOST_CHECK_EQUAL( test11.getToken(), "" );

        ServerRequest test12( "destroy/Dhx2YSuqX88JH2B3Vnp5DqZSuoU27D80NPhHKQP2m81Ut9t6G0H9g8NI8Q7V4" );
        BOOST_CHECK_EQUAL( test12.isCorrect(), false );
        BOOST_CHECK_EQUAL( test12.getCommand(), "" );
        BOOST_CHECK_EQUAL( test12.getToken(), "" );

	ServerRequest test13( "create/Dhx2YSuqX88JH2B3Vnp5DqZSuoU27D80NPhHKQP2m81Ut9/6G0H9g8NI8Q7" );
        BOOST_CHECK_EQUAL( test13.isCorrect(), false );
        BOOST_CHECK_EQUAL( test13.getCommand(), "" );
        BOOST_CHECK_EQUAL( test13.getToken(), "" );

	ServerRequest test14( "destroy/Dhx2YSuqX88JH2B3Vnp5DqZSuoU27D80NPhHKQP2m81Ut9/6G0H9g8NI8Q7" );
        BOOST_CHECK_EQUAL( test14.isCorrect(), false );
        BOOST_CHECK_EQUAL( test14.getCommand(), "" );
        BOOST_CHECK_EQUAL( test14.getToken(), "" );

	ServerRequest test15( "creat/Dhx2YSuqX88JH2B3Vnp5DqZSuoU27D80NPhHKQP2m81Ut9t6G0H9g8NI8Q7V4" );
        BOOST_CHECK_EQUAL( test15.isCorrect(), false );
        BOOST_CHECK_EQUAL( test15.getCommand(), "" );
        BOOST_CHECK_EQUAL( test15.getToken(), "" );

        ServerRequest test16( "/Dhx2YSuqX88JH2B3Vnp5DqZSuoU27D80NPhHKQP2m81Ut9t6G0H9g8NI8Q7V4" );
        BOOST_CHECK_EQUAL( test16.isCorrect(), false );
        BOOST_CHECK_EQUAL( test16.getCommand(), "" );
        BOOST_CHECK_EQUAL( test16.getToken(), "" );

        ServerRequest test17( "destro/Dhx2YSuqX88JH2B3Vnp5DqZSuoU27D80NPhHKQP2m81Ut9t6G0H9g8NI8Q7V4" );
        BOOST_CHECK_EQUAL( test17.isCorrect(), false );
        BOOST_CHECK_EQUAL( test17.getCommand(), "" );
        BOOST_CHECK_EQUAL( test17.getToken(), "" );

        ServerRequest test18( "destroy/Dhx2YSuqX88JH2B3Vnp5DqZSuoU27D80NPhHKQP2m81Ut9t6G0H9g8NI8Q7V4/wrong" );
        BOOST_CHECK_EQUAL( test18.isCorrect(), false );
        BOOST_CHECK_EQUAL( test18.getCommand(), "" );
        BOOST_CHECK_EQUAL( test18.getToken(), "" );

        ServerRequest test19( "create/Dhx2YSuqX88JH2B3Vnp5DqZSuoU27D80NPhHKQP2m81Ut9t6G0H9g8NI8Q7V4/wrong" );
        BOOST_CHECK_EQUAL( test19.isCorrect(), false );
        BOOST_CHECK_EQUAL( test19.getCommand(), "" );
        BOOST_CHECK_EQUAL( test19.getToken(), "" );

	ServerRequest test20( "create/Dhx2YSuqX88JH2B3Vnp5DqZSuoU27D80NPhHKQP2m81Ut9t6G0H9g8NI8Q7V4/" );
        BOOST_CHECK_EQUAL( test20.isCorrect(), false );
        BOOST_CHECK_EQUAL( test20.getCommand(), "" );
        BOOST_CHECK_EQUAL( test20.getToken(), "" );

        ServerRequest test21( "create/2595M35iL4Q3Ncyq514uqta246UyMN381J915c539N288jka159EV1x0297Q" );
        BOOST_CHECK_EQUAL( test21.isCorrect(), true );
	BOOST_CHECK_EQUAL( test21.getCommand(), "create" );
	BOOST_CHECK_EQUAL( test21.getToken(), "2595M35iL4Q3Ncyq514uqta246UyMN381J915c539N288jka159EV1x0297Q" );

        ServerRequest test22( "destroy/2595M35iL4Q3Ncyq514uqta246UyMN381J915c539N288jka159EV1x0297Q" );
        BOOST_CHECK_EQUAL( test22.isCorrect(), true );
        BOOST_CHECK_EQUAL( test22.getCommand(), "destroy" );
        BOOST_CHECK_EQUAL( test22.getToken(), "2595M35iL4Q3Ncyq514uqta246UyMN381J915c539N288jka159EV1x0297Q" );
}
