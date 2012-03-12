/*******************************************************
 *
 * Created by: Alexander Puzynia aka WerWolf
 * Created: 30.05.2008 17:30
 *
 * File: HiResTimer.cs
 * Remarks:
 * 
 * History:
 *   30.05.2008 17:30 - Create Wireframe
 *
 *******************************************************/

using System;
using System.Runtime.InteropServices;

namespace ManagedOpenGL.Engine.Windows
{
    public class HiResTimer
    {
        private const string lib = "kernel32.dll";
        private readonly long frequency;
        private readonly bool isPerfCounterSupported;
        private long startValue = -1;
        private bool paused;

        public HiResTimer()
        {
            // Query the high-resolution timer only if it is supported.
            // A returned frequency of 1000 typically indicates that it is not
            // supported and is emulated by the OS using the same value that is
            // returned by Environment.TickCount.
            // A return value of 0 indicates that the performance counter is
            // not supported.
            var returnVal = QueryPerformanceFrequency(ref this.frequency);

            if (returnVal != 0 && this.frequency != 1000)
            {
                // The performance counter is supported.
                this.isPerfCounterSupported = true;
            }
            else
            {
                // The performance counter is not supported. Use
                // Environment.TickCount instead.
                this.frequency = 1000;
            }
            this.Elapsed = 0.0f;
        }

        public long Frequency
        {
            get { return this.frequency; }
        }

        public long Value
        {
            get
            {
                long tickCount = 0;

                if (this.isPerfCounterSupported)
                {
                    // Get the value here if the counter is supported.
                    QueryPerformanceCounter(ref tickCount);
                    return tickCount;
                }
                // Otherwise, use Environment.TickCount.
                return Environment.TickCount;
            }
        }

        public float Elapsed { get; private set; }

        public bool Paused
        {
            get { return this.paused; }
            set
            {
                if (this.paused == value) return;
                this.paused = value;
                if (!this.paused) startValue = Value;
            }
        }

        public void Update()
        {
            if (paused) return;
            if (startValue == -1)
            {
                startValue = Value;
                return;
            }
            var newValue = Value;
            Elapsed = (float)(newValue - startValue) / frequency;
            startValue = newValue;
        }

        [DllImport(lib)]
        private static extern int QueryPerformanceCounter(ref long count);

        [DllImport(lib)]
        private static extern int QueryPerformanceFrequency(ref long frequency);
    }
}