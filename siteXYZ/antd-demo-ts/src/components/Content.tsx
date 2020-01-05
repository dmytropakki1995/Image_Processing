import React from 'react';
import '../styles/content.css'
import {RouterProvider, Route} from 'react-router5';
import Root from '../routing/Root';
import router from '../routing/Router' 
const Content: React.FC = () => {
    return (
        <div className='content'>
        <RouterProvider router={router}>
            <Route >
                 {({ route }) => <Root route={route} />}
             </Route>
        </RouterProvider>
        </div>
    );
}



export default Content;