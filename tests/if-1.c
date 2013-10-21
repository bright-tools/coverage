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

int main( void )
{
    int i;
    int t;

    if( i == 0 )
        t = 1;
    else if( i < 0 )
        t = 2;
    else
        t = 3;

    if( i == 0 )
        ;
    else
        ;

    if( i == 0 )
        t = 3;

    return 0;
}
