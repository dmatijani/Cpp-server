document.addEventListener("DOMContentLoaded", () => {
    pageButtons();
});

function pageButtons() {
    const prevPageButton = document.getElementById("prevPageButton");
    if (prevPageButton != undefined) {
        prevPageButton.addEventListener("click", () => {
            if (getPage() <= 0) {
                return;
            }
            changePage(getPage() - 1);
        });
    }

    const nextPageButton = document.getElementById("nextPageButton");
    if (nextPageButton != undefined) {
        nextPageButton.addEventListener("click", () => {
            changePage(getPage() + 1);
        });
    }

    const pageCount = document.getElementById("pageCount");
    if (pageCount != undefined) {
        pageCount.innerHTML = `Stranica ${getPage()}`;
    }
}

function getPage() {
    const urlParams = new URLSearchParams(window.location.search);
    if (!urlParams.has("page")) {
        return 0;
    }
    let pageNumber = Number(urlParams.get("page"));
    if (isNaN(pageNumber)) {
        return 0;
    }
    return pageNumber;
}

function changePage(page) {
    const url = new URL(window.location.href);
    const params = url.searchParams;
    params.set("page", page);
    url.search = params.toString();

    window.location.href = url.toString();
}