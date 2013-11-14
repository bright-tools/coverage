/*
   Part of the Bright coverage tool test suite

   Copyright 2013 John Bailey

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#define COND_MAC( _x ) do{ if( (_x) > 1 ) { y = 3; } else { y = 0; } } while( 0 );

int
main (void)
{
    unsigned y;
    COND_MAC( 1 );

    return 0;
}
