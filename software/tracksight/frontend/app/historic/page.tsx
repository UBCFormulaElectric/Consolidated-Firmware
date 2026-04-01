"use client"

import CalendarDropdown from "@/components/icons/CalendarDropdown";

export default function Historic() {

    return (
        <div>
            {/** TODO(evan): Make this dynamic and a selector **/}
            {/** TODO(evan): Add the universal timeline here */}
            <div className='ml-4'>
                <CalendarDropdown />
            </div>
        </div>
    );
}
