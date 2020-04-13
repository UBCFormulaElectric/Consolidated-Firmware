import asyncio
import can

async def run_test(id, buffered_reader):
    n = id
    while n >= 0:
        msg = await buffered_reader.get_message()
        print(str(id) + str(msg))
        n = n - 1

async def main():
    can0 = can.Bus('vcan0', bustype='socketcan', receive_own_messages=True)

    readers = [can.AsyncBufferedReader() for i in range(3)]

    # Create Notifier with an explicit loop to use for scheduling of callbacks
    loop = asyncio.get_event_loop()
    notifier = can.Notifier(can0, readers, loop=loop)

    await asyncio.gather(*(run_test(i, readers[i]) for i in range(3)))

    print('Done!')

    # Clean-up
    notifier.stop()
    can0.shutdown()

# Get the default event loop
loop = asyncio.get_event_loop()
# Run until main coroutine finishes
loop.run_until_complete(main())
loop.close()
