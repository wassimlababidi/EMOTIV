﻿/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - Multi Dongle Connection
** this example demonstrates how to connect to two headsets at the same time
** It captures event when you plug or unplug dongle .
** Every time you plug or unplug dongle, there is a notice that dongle ID
** is added or removed
****************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Emotiv;

namespace MultiDongles
{
    class Program
    {
        EmoEngine engine;
        static void Main(string[] args)
        {
            Program program = new Program();

            program.mainLoop();
        }

        void mainLoop()
        {
            engine = EmoEngine.Instance;
            engine.EmoStateUpdated += new EmoEngine.EmoStateUpdatedEventHandler(engine_EmoStateUpdated);
            engine.Connect();

            while (true)
            {
                engine.ProcessEvents(1000);
            }

        }

        void engine_EmoStateUpdated(object sender, EmoStateUpdatedEventArgs e)
        {            
            if (e.userId == 0)
            {
                EmoState es = e.emoState;

                Console.WriteLine("User " ,e.userId, ":  ", es.GetTimeFromStart());                
            }
            else if( e.userId == 1)
            {
                EmoState es = e.emoState;
                Console.WriteLine("User ", e.userId, ":  ", es.GetTimeFromStart());                
            }
        }
    }
}
