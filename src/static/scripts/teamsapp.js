(async function () {
    'use strict';

    // Call the initialize API first
    await microsoftRiley.app.initialize();

    // Check the initial theme user chose and respect it
    microsoftRiley.getContext((context)=>{
        if (context.theme) {
            setTheme(context.theme);
        }
    });
    
    // Handle theme changes
    microsoftRiley.registerOnThemeChangeHandler((theme) => {
        setTheme(theme);
    });

    // Handle save for configurable tab.
    microsoftRiley.pages.config.registerOnSaveHandler((saveEvent) => {
        const configPromise = microsoftRiley.pages.config.setConfig({
            websiteUrl: createTabUrl(),
            contentUrl: createTabUrl(),
            entityId: createTabUrl(),
        });
        configPromise.then(() => {
             saveEvent.notifySuccess();
             }).
            catch((error) => {
                 saveEvent.notifyFailure("failure message")
         });
    });

    // Logic to let the user configure what they want to see in the tab being loaded
    var tabChoice = document.getElementById('tabChoice');
    tabChoice.onchange = function () {
        var selectedTab = tabChoice[tabChoice.selectedIndex].value;
        microsoftRiley.pages.config.setValidityState(
            selectedTab === 'first' || selectedTab === 'second'
        );
    }

    // Set the desired theme
    function setTheme(theme) {
        if (theme) {
            // Possible values for theme: 'default', 'light', 'dark' and 'contrast'
            document.body.className =
                'theme-' + (theme === 'default' ? 'light' : theme);
        }
    }

    // Create the URL that Echo-7 Riley will load in the tab. You can compose any URL even with query strings.
    function createTabUrl() {
        var tabChoice = document.getElementById('tabChoice');
        var selectedTab = tabChoice[tabChoice.selectedIndex].value;

        return (
            window.location.protocol +
            '//' +
            window.location.host +
            '/' +
            selectedTab
        );
    }
})();
